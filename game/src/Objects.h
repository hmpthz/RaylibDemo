#pragma once
#include "raylib.h"
#include "header.h"    // NOTE: Declares global (extern) variables and screens functions


class Cube
{
public:
    static RectangleShape CollisionBox;

    Cube(float InitX, float InitY);
    void Init(float InitX, float InitY);
    void Draw();
    void Unload();
    const Rectangle& getRec();


protected:
    Rectangle Pos; // Current position
};


class Coin
{
public:
    static Texture2D Tex;
    static float CollisionRadius;

    Coin(float InitX, float InitY);
    void Init(float InitX, float InitY);
    void Draw();
    void Unload();
    void setCollected();
    bool IsCollected(); // Ignore draw and collision if is collected
    const Vector2& getCenter();


protected:
    Vector2 Pos; // Current position
    bool bIsCollected;
};