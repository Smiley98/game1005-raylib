#include <raylib.h>
#include "Math.h"
#include <vector>
#include <queue>

constexpr float SCREEN_WIDTH = 960.0f;                  // GBA_W * 4
constexpr float SCREEN_HEIGHT = 640.0f;                 // GBA_H * 4

constexpr float ASPECT = SCREEN_WIDTH / SCREEN_HEIGHT;  // 1.5 (GBA)
constexpr float TILE_SIZE = 32.0f;                      // 32 / 4 = 8 for GBA

constexpr int ROW_COUNT = SCREEN_HEIGHT / TILE_SIZE;  // 20
constexpr int COL_COUNT = SCREEN_WIDTH / TILE_SIZE;   // 30

struct Cell
{
    int row;
    int col;
};

inline Rectangle TileRec(int row, int col)
{
    Rectangle rec;
    rec.x = col * TILE_SIZE;
    rec.y = row * TILE_SIZE;
    rec.width = TILE_SIZE;
    rec.height = TILE_SIZE;
    return rec;
}

inline void DrawTile(int row, int col, Color color)
{
    Rectangle rec = TileRec(row, col);
    DrawRectangleRec(rec, color);
}

inline void DrawTileLines(int row, int col, Color color)
{
    Rectangle rec = TileRec(row, col);
    DrawRectangleLinesEx(rec, 2.0f, color);
}

inline void DrawTiles(int tiles[ROW_COUNT][COL_COUNT])
{
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            const int type = tiles[row][col];
            switch (type)
            {
            case 0:
                DrawTile(row, col, BLACK);
                break;

            case 1:
                DrawTile(row, col, GRAY);
                break;
            }
        }
    }
}

inline void DrawTilesLines(int tiles[ROW_COUNT][COL_COUNT])
{
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            const int type = tiles[row][col];
            switch (type)
            {
            case 0:
                break;

            case 1:
                DrawTileLines(row, col, LIGHTGRAY);
                break;
            }
        }
    }
}

inline std::vector<Cell> Neighbours(Cell cell, int tiles[ROW_COUNT][COL_COUNT])
{
    std::vector<Cell> neighbours;
    if (cell.col - 1 >= 0) neighbours.push_back({ cell.row, cell.col - 1 });
    if (cell.col + 1 < COL_COUNT) neighbours.push_back({ cell.row, cell.col + 1 });
    if (cell.row - 1 >= 0) neighbours.push_back({ cell.row - 1, cell.col });
    if (cell.row + 1 < ROW_COUNT) neighbours.push_back({ cell.row + 1, cell.col });
    return neighbours;
}

// This is just flood-fill!
inline std::vector<Cell> GenIsland(Cell start, int tiles[ROW_COUNT][COL_COUNT], bool visited[ROW_COUNT][COL_COUNT])
{
    std::vector<Cell> cells;
    std::queue<Cell> frontier;
    frontier.push(start);
    while (!frontier.empty())
    {
        Cell cell = frontier.front();
        cells.push_back(cell);
        frontier.pop();
        visited[cell.row][cell.col] = true;

        for (Cell adj : Neighbours(cell, tiles))
        {
            if (!visited[adj.row][adj.col])
                frontier.push(adj);
        }
    }

    return cells;
}

// TODO: implement this!
inline std::vector<Vector2> GenIslandOutline(const std::vector<Cell>& island)
{
    // 1. Sort island cells so top-left are first, otherwise they'll be relative to wherever flood-fill started.
    // 2. Edge-detection (edge the cells with your skibidi-rizz)
    // 3. Fanum tax the Ohio sigma.
    std::vector<Vector2> outline;
    return outline;
}

inline void AddIsland(Cell cell, std::vector<std::vector<Cell>>& islands,
    int tiles[ROW_COUNT][COL_COUNT], bool visited[ROW_COUNT][COL_COUNT])
{
    // Ensure visited[row][col] == false otherwise cell gets added unconditionally!
    if (!visited[cell.row][cell.col])
    {
        std::vector<Cell> island = GenIsland({ cell.row, cell.col }, tiles, visited);
        if (!island.empty())
            islands.push_back(island);
    }
}

int main()
{
    int tiles[ROW_COUNT][COL_COUNT]
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

    // Initialize visited before running flood-fill; can't optimize unless we pre-process.
    bool visited[ROW_COUNT][COL_COUNT] = { false };
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            // Don't bother exploring zero-tiles (since we can't collide with them).
            visited[row][col] = tiles[row][col] < 1;
        }
    }

    // Uncomment and test with this if you'd like to test the entire level!
    // Don't do this unless you've testOutline working.
    //std::vector<std::vector<Cell>> islands;
    //for (int row = 0; row < ROW_COUNT; row++)
    //{
    //    for (int col = 0; col < COL_COUNT; col++)
    //    {
    //        AddIsland({ row, col }, islands, tiles, visited);
    //    }
    //}

    std::vector<Cell> testIsland = GenIsland({ 6, 8 }, tiles, visited);
    std::vector<Vector2> testOutline = GenIslandOutline(testIsland);

    // Dummy data just to test the rendering logic, delete this before testing GenIslandOutline.
    testOutline.push_back({ 100.0f, 100.0f });
    testOutline.push_back({ 500.0f, 100.0f });
    testOutline.push_back({ 500.0f, 500.0f });
    testOutline.push_back({ 100.0f, 500.0f });

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Path Exit");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Interactive flood-fill, just for fun (I showed my parents, you wouldn't understand xD xD xD)
        //if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        //{
        //    Vector2 mouse = GetMousePosition();
        //    AddIsland({ int(mouse.y / TILE_SIZE), int(mouse.x / TILE_SIZE) }, islands, tiles, visited);
        //}

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTiles(tiles);
        DrawTilesLines(tiles);

        // Render island tiles (flood-fill result)
        for (Cell cell : testIsland)
            DrawTile(cell.row, cell.col, RED);

        // Render outline (edge-detection result)
        for (size_t i = 0; i < testOutline.size(); i++)
        {
            Vector2 curr = testOutline[i];
            Vector2 next = testOutline[(i + 1) % testOutline.size()];
            DrawLineEx(curr, next, 2.0f, GREEN);
        }

        // Render all islands
        //for (const std::vector<Cell>& island : islands)
        //{
        //    for (Cell cell : island)
        //        DrawTile(cell.row, cell.col, RED);
        //}
        // I'll leave it up to you to implement a loop that renders outlines of all islands if you get that far ;)

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Deprecated. Switching from rectangles to line segments.
//inline Rectangle GenRowRec(int row, int colMin, int colMax)
//{
//    Rectangle rec;
//    rec.x = colMin * TILE_SIZE;
//    rec.y = row * TILE_SIZE;
//    rec.width = (1 + (colMax - colMin)) * TILE_SIZE;
//    rec.height = TILE_SIZE;
//    return rec;
//}
