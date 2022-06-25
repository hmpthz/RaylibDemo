/*******************************************************************************************
*
*   raylib game template
*
*   Mock Super Mario Demo
*   Glis Games Hiring Test
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "header.h"    // NOTE: Declares global (extern) variables and screens functions
#include "Objects.h"
#include "Character.h"
#include <string>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------

Font font = { 0 };
Sound fxCoin = { 0 };
float gravity = 600;
Floor ground;


//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------

static const int screenWidth = 800;
static const int screenHeight = 400;
static Character character;
static std::vector<Cube> cubes;
static std::vector<Coin> coins;


//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

// Load global data (assets that must be available in all screens, i.e. font)
static void LoadResources();
// Unload global resources loaded
static void UnloadResources();
// Update and draw one frame
static void MainGameLoop();
static void InitScreen();
static void UpdateScreen();
static void DrawScreen();
// Unload current screen data before closing
static void UnloadScreen();


//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Super Mario Demo");
    InitAudioDevice();      // Initialize audio device
    LoadResources();
    InitScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(MainGameLoop, 60, 1);
#else
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        MainGameLoop();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadScreen();
    UnloadResources();
    CloseAudioDevice();     // Close audio context
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}


//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------
void LoadResources()
{
    font = LoadFont("resources/mecha.png");
    fxCoin = LoadSound("resources/coin.wav");
    Character::Tex = LoadTexture("resources/character.png");
    Coin::Tex = LoadTexture("resources/coin.png");
}

void UnloadResources()
{
    UnloadFont(font);
    UnloadSound(fxCoin);
    UnloadTexture(Character::Tex);
    UnloadTexture(Coin::Tex);
}

void MainGameLoop(void)
{
    // Update
    //----------------------------------------------------------------------------------
    UpdateScreen();
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(WHITE);
        DrawScreen();
        //DrawFPS(10, 10);
    EndDrawing();
    //----------------------------------------------------------------------------------
}

void InitScreen()
{
    ground.left = -999999;
    ground.right = 999999;
    ground.posY = 340;

    // Init cubes
    Cube::CollisionBox.width = 60;
    Cube::CollisionBox.height = 60;
    cubes.push_back(Cube(500, 340));
    cubes.push_back(Cube(380, 280));
    cubes.push_back(Cube(260, 220));
    cubes.push_back(Cube(140, 300));

    // Init coins
    Coin::CollisionRadius = 30;
    coins.push_back(Coin(510, 300));
    coins.push_back(Coin(390, 240));
    coins.push_back(Coin(270, 180));

    // Init character
    Character::MoveVelocity = 100;
    Character::JumpVelocity = 350;
    Character::CollisionBox.width = 30;
    Character::CollisionBox.height = 60;
    character.Init(580, 340);
}

void UpdateScreen()
{
    character.ReceiveInput();
    character.Update(cubes, coins);
}

void DrawScreen()
{
    for (auto& cube : cubes)
    {
        cube.Draw();
    }
    for (auto& coin : coins)
    {
        coin.Draw();
    }
    character.Draw();

    std::string coinText = "COINS: " + std::to_string(character.getCoins());
    DrawText(coinText.c_str(), 20, 20, 20, BLACK);
}

void UnloadScreen()
{
    for (auto& cube : cubes)
    {
        cube.Unload();
    }
    for (auto& coin : coins)
    {
        coin.Unload();
    }
    character.Unload();
}
