#include "raylib.h"
#include "Math.h"

constexpr float SCREEN_WIDTH = 1200.0f;
constexpr float SCREEN_HEIGHT = 800.0f;
constexpr Vector2 CENTER{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

enum State : int
{
    BEGIN,
    PLAY,
    END
};

struct Game
{
    Rectangle button;
    Color buttonColor;
    int state;
};

void UpdateBegin(Game& game)
{
    // Add mouse-out vs mouse-over colour
    Color buttonColorIn = GRAY;
    Color buttonColorOut = DARKGRAY;
    bool mouseOver = CheckCollisionPointRec(GetMousePosition(), game.button);
    game.buttonColor = mouseOver ? buttonColorIn : buttonColorOut;
    if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        ++game.state %= 3;
}

void UpdatePlay(Game& game)
{
    // Replace this with actual win condition to transition from pong play to pong game over
    if (CheckCollisionPointRec(GetMousePosition(), game.button) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        ++game.state %= 3;
}

void UpdateEnd(Game& game)
{
    // Replace with a play again vs quit option?
    if (CheckCollisionPointRec(GetMousePosition(), game.button) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        ++game.state %= 3;
}

void DrawBegin(Game& game)
{
    DrawRectangleRec(game.button, game.buttonColor);
}

void DrawPlay(Game& game)
{
    DrawRectangleRec(game.button, GREEN);
}

void DrawEnd(Game& game)
{
    DrawRectangleRec(game.button, BLUE);
}

int main()
{
    Game game;
    game.state = BEGIN;
    game.button.x = 550;
    game.button.y = 350;
    game.button.width = 100;
    game.button.height = 50;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // State change test
        if (IsKeyPressed(KEY_SPACE))
        {
            ++game.state %= 3;
        }

        switch (game.state)
        {
        case BEGIN:
            UpdateBegin(game);
            break;

        case PLAY:
            UpdatePlay(game);
            break;

        case END:
            UpdateEnd(game);
            break;
        }

        BeginDrawing();
        switch (game.state)
        {
        case BEGIN:
            DrawBegin(game);
            break;

        case PLAY:
            DrawPlay(game);
            break;

        case END:
            DrawEnd(game);
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
