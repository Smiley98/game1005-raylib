#include "raylib.h"
#include "Math.h"
bool gameOn = true;
constexpr float SCREEN_WIDTH = 1200.0f;
constexpr float SCREEN_HEIGHT = 800.0f;
constexpr Vector2 CENTER{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

// Ball can move half the screen width per-second
constexpr float BALL_SPEED = SCREEN_WIDTH * 0.5f;
constexpr float BALL_SIZE = 40.0f;

// Paddles can move half the screen height per-second
constexpr float PADDLE_SPEED = SCREEN_HEIGHT * 0.5f;
constexpr float PADDLE_WIDTH = 40.0f;
constexpr float PADDLE_HEIGHT = 80.0f;


//dumbest way to do this but hear me out
struct Player
{
    int score;
    Color textCol;
    const char* name;
};

void GameOver(Player winner) {

    gameOn = false;
    //just covers shit up so is probably disastrously bad
    DrawText(TextFormat("Winner: %s", winner.name), (SCREEN_WIDTH * 0.5f) / 2, SCREEN_HEIGHT * 0.5f, 50, winner.textCol);

}



struct Box
{
    float xMin;
    float xMax;
    float yMin;
    float yMax;
};

bool BoxOverlap(Box box1, Box box2)
{
    bool x = box1.xMax >= box2.xMin && box1.xMin <= box2.xMax;
    bool y = box1.yMax >= box2.yMin && box1.yMin <= box2.yMax;
    return x && y;
}

Rectangle BoxToRec(Box box)
{
    Rectangle rec;
    rec.x = box.xMin;
    rec.y = box.yMin;
    rec.width = box.xMax - box.xMin;
    rec.height = box.yMax - box.yMin;
    return rec;
}

Box BallBox(Vector2 position)
{
    Box box;
    box.xMin = position.x - BALL_SIZE * 0.5f;
    box.xMax = position.x + BALL_SIZE * 0.5f;
    box.yMin = position.y - BALL_SIZE * 0.5f;
    box.yMax = position.y + BALL_SIZE * 0.5f;
    return box;
}

Box PaddleBox(Vector2 position)
{
    Box box;
    box.xMin = position.x - PADDLE_WIDTH * 0.5f;
    box.xMax = position.x + PADDLE_WIDTH * 0.5f;
    box.yMin = position.y - PADDLE_HEIGHT * 0.5f;
    box.yMax = position.y + PADDLE_HEIGHT * 0.5f;
    return box;
}

void ResetBall(Vector2& position, Vector2& direction)
{
    position = CENTER;
    direction.x = rand() % 2 == 0 ? -1.0f : 1.0f;
    direction.y = 0.0f;
    direction = Rotate(direction, Random(100.0f, 360.0f) * DEG2RAD);
}

void DrawBall(Vector2 position, Color color)
{
    Box ballBox = BallBox(position);
    DrawRectangleRec(BoxToRec(ballBox), color);
}

void DrawPaddle(Vector2 position, Color color)
{
    Box paddleBox = PaddleBox(position);
    DrawRectangleRec(BoxToRec(paddleBox), color);
}

int main()
{
    Rectangle textBox = { SCREEN_WIDTH * 0.5f - 100, 180 , 225, 50 };
    bool mouseOnText = false;

    //player init?

    Player player1;
    Player player2;
    player1.textCol = BLUE;
    player2.textCol = RED;
    player2.score = 0;
    player1.score = 0;
    player1.name = "PLAYER1";
    player2.name = "PLAYER2";


    Vector2 ballPosition;
    Vector2 ballDirection;
    ResetBall(ballPosition, ballDirection);

    Vector2 paddle1Position, paddle2Position;
    paddle1Position.x = SCREEN_WIDTH * 0.05f;
    paddle2Position.x = SCREEN_WIDTH * 0.95f;
    paddle1Position.y = paddle2Position.y = CENTER.y;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        float ballDelta = BALL_SPEED * dt;
        float paddleDelta = PADDLE_SPEED * dt;

        // Move paddle1 with WS key input
        if (IsKeyDown(KEY_W))
            paddle1Position.y -= paddleDelta;
        if (IsKeyDown(KEY_S))
            paddle1Position.y += paddleDelta;

        // Move paddle2 with I/K key input
        if (IsKeyDown(KEY_I))
            paddle2Position.y -= paddleDelta;
        if (IsKeyDown(KEY_K))
            paddle2Position.y += paddleDelta;

        float phh = PADDLE_HEIGHT * 0.5f;
        paddle1Position.y = Clamp(paddle1Position.y, phh, SCREEN_HEIGHT - phh);
        paddle2Position.y = Clamp(paddle2Position.y, phh, SCREEN_HEIGHT - phh);

        // Change the ball's direction on-collision
        Vector2 ballPositionNext = ballPosition + ballDirection * ballDelta;
        Box ballBox = BallBox(ballPositionNext);
        Box paddle1Box = PaddleBox(paddle1Position);
        Box paddle2Box = PaddleBox(paddle2Position);

        //handling scores
        if (ballBox.xMin < -2.0f) {
            player2.score += 1;
            if (player2.score >= 5) {
                GameOver(player2);
                ClearBackground(BLACK);

            }
            else {
                ballDirection.x *= -1.0f;
                ResetBall(ballPosition, ballDirection);
            }
        }
        else if (ballBox.xMax > SCREEN_WIDTH) {
            player1.score += 1;
            if (player1.score >= 5) {
                GameOver(player1);
                ClearBackground(BLACK);

            }
            else {
                ballDirection.x *= -1.0f; //seems to give the behaviour i want but I think im deluding myself
                //sometimes goes straight up and down?
                ResetBall(ballPosition, ballDirection);
            }
        }


        if (ballBox.yMin < 0.0f || ballBox.yMax > SCREEN_HEIGHT)
            ballDirection.y *= -1.0f;
        if (BoxOverlap(ballBox, paddle1Box) || BoxOverlap(ballBox, paddle2Box))
            ballDirection.x *= -1.0f;

        // Update ball position after collision resolution, then render
        ballPosition = ballPosition + ballDirection * ballDelta;


        BeginDrawing();




        //textytext stuff
        if (gameOn) {
            DrawText(player1.name, 250, 20, 50, player1.textCol);
            DrawText(TextFormat("SCORE: %d", player1.score), 250, 90, 50, player1.textCol);
            //%d is int placeholder
            DrawText(player2.name, 650, 20, 50, player2.textCol);
            DrawText(TextFormat("SCORE: %d", player2.score), 650, 90, 50, player2.textCol);
            ClearBackground(BLACK);
            DrawBall(ballPosition, WHITE);
            DrawPaddle(paddle1Position, WHITE);
            DrawPaddle(paddle2Position, WHITE);
        }
        else {
            DrawText("NICE!", 250, 20, 50, YELLOW);
        }


        EndDrawing();




    }

    CloseWindow();
    return 0;
}
