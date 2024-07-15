#include <raylib.h>
#include "Math.h"
#include <array>
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

constexpr std::array<Cell, 4> DIRECTIONS{ Cell{ -1, 0 }, Cell{ 1, 0}, Cell{ 0, -1 }, Cell{ 0, 1 } };

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

inline bool InBounds(Cell cell, int rows = ROW_COUNT, int cols = COL_COUNT)
{
    return cell.col - 1 >= 0 && cell.col + 1 < cols && cell.row - 1 >= 0 && cell.row + 1 < rows;
}

inline bool IsExtent(Cell cell, int rows = ROW_COUNT, int cols = COL_COUNT)
{
    return cell.col == 0 || cell.col == cols - 1 || cell.row == 0 || cell.row == rows - 1;
}

inline bool IsEdge(Cell cell, const Tiles& tiles)
{
    if (IsExtent(cell))
        return true;

    for (Cell dir : DIRECTIONS)
    {
        Cell adj = { cell.row + dir.row, cell.col + dir.col };
        if (InBounds(adj) && tiles[adj.row][adj.col] == 0)
            return true;
    }

    return false;
}

inline std::vector<Cell> GenIsland(Cell start, const Tiles& tiles, Visited& visited)
{
    std::vector<Cell> island;

    std::queue<Cell> frontier;
    frontier.push(start);
    visited[start.row][start.col] = true;

    while (!frontier.empty())
    {
        Cell cell = frontier.front();
        frontier.pop();
        island.push_back(cell);

        for (Cell dir : DIRECTIONS)
        {
            Cell adj = { cell.row + dir.row, cell.col + dir.col };
            if (!InBounds(adj)) continue;

            bool canVist = !visited[adj.row][adj.col];
            bool canCollide = tiles[adj.row][adj.col] > 0;
            if (canVist && canCollide)
            {
                frontier.push(adj);
                visited[adj.row][adj.col] = true;
            }
        }
    }

    return island;
}

inline std::vector<Cell> GenEdges(const std::vector<Cell>& island, const Tiles& tiles)
{
    std::vector<Cell> edges;
    for (Cell cell : island)
    {
        if (IsEdge(cell, tiles))
            edges.push_back(cell);
    }
    return edges;
}

// 1. An edge can contain min 1 line max 4 lines. Compare adjacent tile values to figure out how many lines per edge.
// 2. Order the lines to connect to each other.
// 3. Check if adjacent lines have the same slope. If so, they must be unified into a single line.
inline std::vector<Vector2> GenOutline(const std::vector<Cell>& edges, const Tiles& tiles)
{
    std::vector<Vector2> lines;
    return lines;
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
    {                              //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 19
        std::array<int, COL_COUNT> { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },// 0
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 1
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 2
        std::array<int, COL_COUNT> { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },// 3
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

    std::array<std::array<bool, COL_COUNT>, ROW_COUNT> visited = { false };
    for (int row = 0; row < ROW_COUNT; row++)
    {
        for (int col = 0; col < COL_COUNT; col++)
        {
            visited[row][col] = tiles[row][col] == 0;
        }
    }

    std::vector<Cell> testIsland = GenIsland({ 7, 6 }, tiles, visited);
    std::vector<Cell> testEdges = GenEdges(testIsland, tiles);
    std::vector<Vector2> testOutline = GenOutline(testEdges, tiles);

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

        for (Cell cell : testEdges)
            DrawTile(cell.row, cell.col, GREEN);

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
