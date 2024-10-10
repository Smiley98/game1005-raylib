#include "raylib.h"
#include "Math.h"

constexpr float SCREEN_WIDTH = 1200.0f;
constexpr float SCREEN_HEIGHT = 800.0f;
constexpr Vector2 CENTER{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

struct Game
{

};

enum State : int
{
    BEGIN,
    PLAY,
    END
};

void UpdateBegin(Game& game)
{

}

void UpdatePlay(Game& game)
{

}

void UpdateEnd(Game& game)
{

}

void DrawBegin(Game& game)
{
    DrawRectangle(550, 350, 100, 50, RED);
}

void DrawPlay(Game& game)
{
    DrawRectangle(550, 350, 100, 50, GREEN);
}

void DrawEnd(Game& game)
{
    DrawRectangle(550, 350, 100, 50, BLUE);
}

int main()
{
    Game game;
    int state = BEGIN;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // State change test
        if (IsKeyPressed(KEY_SPACE))
        {
            ++state %= 3;
        }

        switch (state)
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
        switch (state)
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
