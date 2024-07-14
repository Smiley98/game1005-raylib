#include <raylib.h>
#include "Math.h"
#include <array>
#include <vector>
#include <queue>
#include <algorithm>

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

using Tiles = std::array<std::array<int, COL_COUNT>, ROW_COUNT>;
using Visited = std::array<std::array<bool, COL_COUNT>, ROW_COUNT>;

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

inline void DrawTiles(const Tiles& tiles)
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

inline void DrawTilesLines(const Tiles& tiles)
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

inline std::vector<Cell> Neighbours(Cell cell, const Tiles& tiles)
{
    std::vector<Cell> neighbours;
    if (cell.col - 1 >= 0) neighbours.push_back({ cell.row, cell.col - 1 });
    if (cell.col + 1 < COL_COUNT) neighbours.push_back({ cell.row, cell.col + 1 });
    if (cell.row - 1 >= 0) neighbours.push_back({ cell.row - 1, cell.col });
    if (cell.row + 1 < ROW_COUNT) neighbours.push_back({ cell.row + 1, cell.col });
    return neighbours;
}

inline std::vector<Cell> GenIsland(Cell start, const Tiles& tiles, Visited& visited)
{
    std::vector<Cell> cells;
    std::queue<Cell> frontier;
    frontier.push(start);
    visited[start.row][start.col] = true;
    while (!frontier.empty())
    {
        Cell cell = frontier.front();
        frontier.pop();
        cells.push_back(cell);

        for (Cell adj : Neighbours(cell, tiles))
        {
            bool canVist = !visited[adj.row][adj.col];
            bool canCollide = tiles[adj.row][adj.col] > 0;
            if (canVist && canCollide)
            {
                frontier.push(adj);
                visited[adj.row][adj.col] = true;
            }
        }
    }

    return cells;
}

std::vector<Cell> SortMin(std::vector<Cell> island)
{
    std::sort(island.begin(), island.end(), [](Cell a, Cell b) {
        return a.col < b.col;
        });
    return island;
}

inline std::vector<Vector2> GenIslandOutline(const std::vector<Cell>& island)
{
    std::vector<Vector2> outline;
    return outline;
}

inline void AddIsland(Cell cell, std::vector<std::vector<Cell>>& islands,
    const Tiles& tiles, Visited& visited)
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
    std::array<std::array<int, COL_COUNT>, ROW_COUNT> tiles =
    {
        std::array<int, COL_COUNT> { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        std::array<int, COL_COUNT> { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

    std::array<std::array<bool, COL_COUNT>, ROW_COUNT> visited = { false };
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            visited[row][col] = tiles[row][col] == 0;
        }
    }

    // row 8 col 7 is the bottom-left clel of the test island.
    std::vector<Cell> testIsland = GenIsland({ 8, 7 }, tiles, visited);
    //testIsland = SortMin(testIsland);
    std::vector<Vector2> testOutline = GenIslandOutline(testIsland);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Path Exit");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTiles(tiles);
        DrawTilesLines(tiles);

        for (Cell cell : testIsland)
            DrawTile(cell.row, cell.col, RED);

        for (size_t i = 0; i < testOutline.size(); i++)
        {
            Vector2 curr = testOutline[i];
            Vector2 next = testOutline[(i + 1) % testOutline.size()];
            DrawLineEx(curr, next, 2.0f, GREEN);
        }

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
