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
                x = GetScreenWidth() * 0.25f;
                y = GetScreenHeight() * 0.5f;
            }
        }

        // Move using mouse position if we're moving based on mouse-input
        if (useMouse)
        {
            Vector2 mouse = GetMousePosition();
            x = mouse.x;
            y = mouse.y;
        }
        else
        {
            // Move using time-based animation otherwise!
            float dt = GetFrameTime();
            x += speed * dt;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircle(x, y, 25.0f, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
