#include "raylib.h"
#include "Math.h"

int main()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);

    // Horizontal start at 25% of the screen width
    float x = GetScreenWidth() * 0.25f;

    // Vertical position of 50% of the screen height
    float y = GetScreenHeight() * 0.5f;

    // Move at 100 pixels per second
    float speed = 100.0f;

    while (!WindowShouldClose())
    {
        // Time from previous to current frame (AKA Unity's Time.deltaTime)
        float dt = GetFrameTime();

        // Convert from 100 pixels per frame to 100 pixels per second, then increment x-positon
        x += speed * dt;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircle(x, y, 25.0f, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
