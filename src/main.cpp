#include <raylib.h>
#include "Math.h"
#include "Generation.h"
#include "Physics.h"

struct Scene
{
    std::vector<std::vector<Cell>> islands;
    std::vector<std::vector<Cell>> edges;
    std::vector<std::vector<Line>> lines;
};

struct Player
{
    Vector2 pos{};
    Rigidbody rb{};
    
    const float dragX = 0.05f;
    const float maxVelX = 500.0f;

    const float defaultGravityScale = 2.0f;
    float gravityScale = defaultGravityScale;

    const int jumpCountMax = 2;
    int jumpCount = jumpCountMax;

    const float colliderRadius = 15.0f;
    const float groundColliderRadius = 2.5f;
    bool grounded = false;

    bool collisionPrev = false;
    bool collisionCurr = false;
};

bool CheckCollisionLevel(Vector2 position, float radius, const Scene& scene, std::vector<Vector2>* mtvs = nullptr);
void GenScene(Scene& scene, const Tiles& tiles);
void DrawScene(const Scene& scene);

int main()
{
    std::array<std::array<int, COL_COUNT>, ROW_COUNT> tiles =
    {                              //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 19
        std::array<int, COL_COUNT> { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },// 0
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 1
        std::array<int, COL_COUNT> { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 2
        std::array<int, COL_COUNT> { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 3
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },// 4
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },// 5
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },// 6
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },// 7
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 8
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 9
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 10
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 11
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 12
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 13
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },// 14
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },// 15
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },// 16
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 17
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 18
        std::array<int, COL_COUNT> { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } // 19
    };

    Scene scene;
    GenScene(scene, tiles);

    Player player;
    player.pos = { SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.75f };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Path Exit");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_A))
        {
            player.rb.acc.x = -1000.0f;
        }
        else if (IsKeyDown(KEY_D))
        {
            player.rb.acc.x = 1000.0f;
        }
        else
        {
            player.rb.acc.x = 0.0f;
        }
        if (IsKeyPressed(KEY_W) && player.jumpCount > 0)
        {
            player.jumpCount--;
            player.rb.vel.y = -250.0f;
            player.grounded = false;
        }

        player.gravityScale = player.grounded ? 0.0f : player.defaultGravityScale;
        player.rb.acc.y = GRAVITY.y * player.gravityScale;
        player.rb.vel = Integrate(player.rb.vel, player.rb.acc, dt);
        player.rb.vel.x *= powf(player.dragX, dt);
        player.rb.vel.x = Clamp(player.rb.vel.x, -player.maxVelX, player.maxVelX);
        player.pos = Integrate(player.pos, player.rb.vel, dt);

        Vector2 groundCollider = player.pos + V2_DOWN * (player.colliderRadius - player.groundColliderRadius);
        player.grounded = CheckCollisionLevel(groundCollider, player.groundColliderRadius, scene);

        std::vector<Vector2> mtvs;
        player.collisionCurr = CheckCollisionLevel(player.pos, player.colliderRadius, scene, &mtvs);

        for (Vector2 mtv : mtvs)
        {
            // "On collision enter"
            if (!player.collisionPrev && player.collisionCurr)
            {
                Vector2 normal = Normalize(mtv);
                const float mag = 0.9f;

                // Vertical collision
                if (fabsf(normal.y) >= mag)
                    player.rb.vel.y = 0.0f;
                
                // Horizontal collision
                if (fabsf(normal.x) >= mag)
                    player.rb.vel.x = 0.0f;
            }

            // "On collision stay"
            player.pos = player.pos + mtv;
        }

        // Note that this won't fire if player travels an extremely short distance because it goes from
        // previously colliding with the ground to colliding with the ceiling in 1 frame
        // (!true && true) --> no "on collision enter"!
        // Solve by storing object ids (including null id for no collision) and comparing if prevId == currId
        player.collisionPrev = player.collisionCurr;
        player.jumpCount = player.grounded ? player.jumpCountMax : player.jumpCount;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTiles(tiles);
        DrawTilesLines(tiles);
        DrawScene(scene);
        DrawCircleV(player.pos, player.colliderRadius, LIGHTGRAY);
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

bool CheckCollisionLevel(Vector2 position, float radius, const Scene& scene, std::vector<Vector2>* mtvs)
{
    if (mtvs == nullptr)
    {
        for (const std::vector<Line>& lines : scene.lines)
        {
            for (const Line& line : lines)
            {
                if (LineCircle(line.start, line.end, position, radius))
                    return true;
            }
        }
        return false;
    }
    else
    {
        mtvs->reserve(4);
        for (const std::vector<Line>& lines : scene.lines)
        {
            for (const Line& line : lines)
            {
                Vector2 mtv{};
                if (LineCircle(line.start, line.end, position, radius, &mtv))
                    mtvs->push_back(mtv);
            }
        }
        return !mtvs->empty();
    }
}

void GenScene(Scene& scene, const Tiles& tiles)
{
    // Visited pre-pass
    std::array<std::array<bool, COL_COUNT>, ROW_COUNT> visited = { false };
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            visited[row][col] = tiles[row][col] < 1;
        }
    }

    // Generate islands
    std::vector<std::vector<Cell>> islands;
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            if (!visited[row][col])
            {
                std::vector<Cell> island = GenIsland({ row, col }, tiles);
                if (!island.empty())
                {
                    islands.push_back(island);
                    for (Cell cell : island)
                        visited[cell.row][cell.col] = true;
                }
            }
        }
    }

    // Generate edges
    std::vector<std::vector<Cell>> edges;
    for (const std::vector<Cell>& island : islands)
        edges.push_back(GenEdges(island, tiles));

    // Generate lines
    std::vector<std::vector<Line>> lines;
    for (const std::vector<Cell>& edge : edges)
        lines.push_back(GenOutline(edge, tiles));

    scene.islands = islands;
    scene.edges = edges;
    scene.lines = lines;
}

void DrawScene(const Scene& scene)
{
    for (const std::vector<Cell>& island : scene.islands)
    {
        for (Cell cell : island)
            DrawTile(cell.row, cell.col, GREEN);
    }

    for (const std::vector<Cell>& edges : scene.edges)
    {
        for (Cell cell : edges)
            DrawTile(cell.row, cell.col, RED);
    }

    for (const std::vector<Line>& lines : scene.lines)
    {
        for (Line line : lines)
            DrawLineEx(line.start, line.end, 2.0f, SKYBLUE);
    }
}
