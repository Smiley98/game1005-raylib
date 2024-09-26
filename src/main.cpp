#include "raylib.h"
#include "Math.h"

int main()
{
    InitWindow(800, 800, "Game");
    SetTargetFPS(60);

    // Position of our circle
    Vector2 position;
    position.x = GetScreenWidth() * 0.25f;
    position.y = GetScreenHeight() * 0.5f;

    // Move at 100 pixels per second
    float speed = 100.0f;

    // Time-based movement if false, mouse-based movement if true
    bool useMouse = false;

    while (!WindowShouldClose())
    {
        // Toggle mouse input on/off when we press space
        if (IsKeyPressed(KEY_SPACE))
        {
            useMouse = !useMouse;

            // Reset position if we switch back to time-based animation
            if (!useMouse)
            {
                position.x = GetScreenWidth() * 0.25f;
                position.y = GetScreenHeight() * 0.5f;
            }
        }

        // Move using mouse position if we're moving based on mouse-input
        if (useMouse)
        {
            position = GetMousePosition();
        }
        else
        {
            // Move using time-based animation otherwise!
            float dt = GetFrameTime();
            position.x += speed * dt;
        }

        // Optional homework: use vector math to move the circle in different directions!
        // The Normalize and Rotate functions may be of use!!
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(position, 25.0f, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
