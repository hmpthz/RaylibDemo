#include "Character.h"
#include <iostream>

float Character::MoveVelocity;
float Character::JumpVelocity;
Texture2D Character::Tex;
RectangleShape Character::CollisionBox;


void Character::Init(float InitX, float InitY)
{
    Pos.width = CollisionBox.width;
    Pos.height = CollisionBox.height;
    Pos.x = InitX;
    Pos.y = InitY;
    Velocity.x = 0;
    Velocity.y = 0;
    Mode = MovementMode::GROUND;
    floor = ground;
    CoinCount = 0;
}

void Character::ReceiveInput()
{
    if (IsKeyDown(KEY_A)) Velocity.x = -MoveVelocity;
    else if (IsKeyDown(KEY_D)) Velocity.x = MoveVelocity;
    else Velocity.x = 0;

    if (Mode == MovementMode::GROUND && IsKeyPressed(KEY_W))
    {
        Velocity.y = -JumpVelocity;
        Mode = MovementMode::FALLING;
    }
}

void Character::Update(std::vector<Cube>& cubes, std::vector<Coin>& coins)
{
    float DeltaTime = GetFrameTime();
    // Move
    Rectangle NextPos = Pos;
    NextPos.x += DeltaTime * Velocity.x;
    NextPos.y += DeltaTime * Velocity.y;

    // Simulate Physics
    if (Mode == MovementMode::GROUND)
    {
        SimulateGroundMoving(NextPos, DeltaTime, cubes);
    }
    else
    {
        SimulateFalling(NextPos, DeltaTime, cubes);
    }
    // Set new position
    Pos.x = NextPos.x;
    Pos.y = NextPos.y;

    // Collect coins
    for (auto& coin : coins)
    {
        if (coin.IsCollected()) continue;
        if (CheckCollisionCircleRec(coin.getCenter(), Coin::CollisionRadius, Pos))
        {
            CoinCount++;
            coin.setCollected();
            PlaySound(fxCoin);
        }
    }
}

void Character::SimulateGroundMoving(Rectangle& NextPos, float DeltaTime, std::vector<Cube>& cubes)
{
    // Constrain within screen
    if (NextPos.x < 0) NextPos.x = 0;
    else if (NextPos.x > GetScreenWidth() - CollisionBox.width) NextPos.x = GetScreenWidth() - CollisionBox.width;

    // Check collision
    for (auto& cube : cubes)
    {
        if (!CheckCollisionRecs(NextPos, cube.getRec())) continue;
        // has collision
        NextPos.x = Pos.x;
        break;
    }

    // Check if needs falling
    if (NextPos.x < floor.left || NextPos.x > floor.right)
    {
        Mode = MovementMode::FALLING;
    }
}

void Character::SimulateFalling(Rectangle& NextPos, float DeltaTime, std::vector<Cube>& cubes)
{
    // Constrain within screen
    if (NextPos.x < 0) NextPos.x = 0;
    else if (NextPos.x > GetScreenWidth() - CollisionBox.width) NextPos.x = GetScreenWidth() - CollisionBox.width;
    if (NextPos.y < 0)
    {
        NextPos.y = 0;
        // Clear upward velocity
        Velocity.y = 0;
    }
    else if (NextPos.y > GetScreenHeight() - CollisionBox.height)
    {
        NextPos.y = GetScreenHeight() - CollisionBox.height;
        // Landed on the ground
        Velocity.y = 0;
        floor = ground;
        Pos.y = floor.posY;
        Mode = MovementMode::GROUND;
    }

    // Check collision
    for (auto& cube : cubes)
    {
        Rectangle CubeRec = cube.getRec();

        if (!CheckCollisionRecs(NextPos, CubeRec)) continue;
        // has collision
        if (Velocity.y >= 0)
        {
            // Is falling, check landed floor
            NextPos.x = Pos.x;

            // Landed floor mechanic ------------------------
            Floor NewFloor = getFloorFromCube(CubeRec);
            if ((NextPos.y + CollisionBox.height < CubeRec.y + Cube::CollisionBox.height / 3)
                && (NewFloor.left < NextPos.x && NewFloor.right > NextPos.x))
            {
                Velocity.y = 0;
                floor = NewFloor;
                NextPos.y = floor.posY;
                Mode = MovementMode::GROUND;
            }
        }
        else
        {
            // Is jumping, check ceiling
            Velocity.y = 0;
            NextPos.x = Pos.x;
            NextPos.y = Pos.y;
        }
        break;
    }

    // Gravity falling
    if (Mode == MovementMode::FALLING)
    {
        Velocity.y += DeltaTime * gravity;
    }
}

Floor Character::getFloorFromCube(const Rectangle& CubeRec)
{
    Floor NewFloor;
    NewFloor.posY = CubeRec.y - CollisionBox.height;
    NewFloor.left = CubeRec.x - CollisionBox.width;
    NewFloor.right = CubeRec.x + Cube::CollisionBox.width;
    return NewFloor;
}

void Character::Draw()
{
    DrawTexture(Tex, Pos.x, Pos.y, WHITE);
}

void Character::Unload()
{

}

int Character::getCoins()
{
    return CoinCount;
}
