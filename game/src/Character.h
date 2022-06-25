#pragma once
#include "raylib.h"
#include "header.h"    // NOTE: Declares global (extern) variables and screens functions
#include "Objects.h"
#include <vector>


enum class MovementMode
{
    GROUND, // On the ground, can move left right or jump
    FALLING // Can only move left right
};

class Character
{
public:
    static float MoveVelocity; // Move velocity along X-axis
    static float JumpVelocity; // Initial jump velocity along Y-axis
    static Texture2D Tex;
    static RectangleShape CollisionBox;

    void Init(float InitX, float InitY);
    void ReceiveInput();
    void Update(std::vector<Cube>& cubes, std::vector<Coin>& coins);
    void SimulateGroundMoving(Rectangle& NextPos, float DeltaTime, std::vector<Cube>& cubes);
    void SimulateFalling(Rectangle& NextPos, float DeltaTime, std::vector<Cube>& cubes);
    Floor getFloorFromCube(const Rectangle& CubeRec);
    void Draw();
    void Unload();
    int getCoins();


protected:
    Rectangle Pos; // Current position
    Vector2 Velocity; // Velocity for physics simulation
    MovementMode Mode;
    Floor floor;
    int CoinCount;
};