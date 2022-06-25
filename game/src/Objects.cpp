#include "Objects.h"

RectangleShape Cube::CollisionBox;
Texture2D Coin::Tex;
float Coin::CollisionRadius;


Cube::Cube(float InitX, float InitY)
{
    Init(InitX, InitY);
}

void Cube::Init(float InitX, float InitY)
{
    Pos.width = CollisionBox.width;
    Pos.height = CollisionBox.height;
    Pos.x = InitX;
    Pos.y = InitY;
}

void Cube::Draw()
{
    DrawRectangleLinesEx(Pos, 2.0, BLACK);
}

void Cube::Unload()
{

}

const Rectangle& Cube::getRec()
{
    return Pos;
}


Coin::Coin(float InitX, float InitY)
{
    Init(InitX, InitY);
}

void Coin::Init(float InitX, float InitY)
{
    Pos.x = InitX;
    Pos.y = InitY;
    bIsCollected = false;
}

void Coin::Draw()
{
    if (bIsCollected) return;
    DrawTexture(Tex, Pos.x, Pos.y, WHITE);
}

void Coin::Unload()
{

}

void Coin::setCollected()
{
    bIsCollected = true;
}

bool Coin::IsCollected()
{
    return bIsCollected;
}

const Vector2& Coin::getCenter()
{
    return Pos;
}
