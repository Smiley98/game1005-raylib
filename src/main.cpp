#include <raylib.h>
#include "Math.h"

#include <cassert>
#include <algorithm>

#include <array>
#include <vector>

#include <unordered_set>
#include <queue>

constexpr float SCREEN_WIDTH = 960.0f;
constexpr float SCREEN_HEIGHT = 640.0f;

constexpr float ASPECT = SCREEN_WIDTH / SCREEN_HEIGHT;
constexpr float TILE_SIZE = 32.0f;

constexpr int ROW_COUNT = SCREEN_HEIGHT / TILE_SIZE;
constexpr int COL_COUNT = SCREEN_WIDTH / TILE_SIZE;

struct Cell
{
    int row;
    int col;

    bool operator==(Cell cell) const
    {
        return row == cell.row && col == cell.col;
    }
};

struct CellHash
{
    size_t operator()(Cell cell) const
    {
        int rowHash = std::hash<int>()(cell.row);
        int colHash = std::hash<int>()(cell.col) << 1;
        return rowHash ^ colHash;
    }
};

constexpr std::array<Cell, 4> DIRECTIONS{ Cell{ -1, 0 }, Cell{ 1, 0}, Cell{ 0, -1 }, Cell{ 0, 1 } };
using Tiles = std::array<std::array<int, COL_COUNT>, ROW_COUNT>;

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
    return cell.col >= 0 && cell.col < cols && cell.row >= 0 && cell.row < rows;
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

inline std::vector<Cell> GenIsland(Cell start, const Tiles& tiles)
{
    assert(tiles[start.row][start.col] > 0);
    std::vector<Cell> island;

    std::unordered_set<Cell, CellHash> closed;
    std::queue<Cell> open;
    open.push(start);

    while (!open.empty())
    {
        Cell cell = open.front();
        open.pop();

        if (closed.find(cell) != closed.end())
            continue;

        closed.insert(cell);
        island.push_back(cell);

        for (Cell dir : DIRECTIONS)
        {
            Cell adj = { cell.row + dir.row, cell.col + dir.col };
            bool add = InBounds(adj) && tiles[adj.row][adj.col] > 0;
            if (add)
                open.push(adj);
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

struct Line
{
    Vector2 start;
    Vector2 end;
};

bool IsHorizontal(Line line)
{
    return Equals(line.start.y, line.end.y);
}

bool IsVertical(Line line)
{
    return Equals(line.start.x, line.end.x);
}

bool CanMerge(Line a, Line b)
{
    // If horizontal and same row
    if (IsHorizontal(a) && IsHorizontal(b) && Equals(a.start.y, b.start.y))
        return Equals(a.end.x, b.start.x) || Equals(a.start.x, b.end.x);

    // If vertical and same column
    if (IsVertical(a) && IsVertical(b) && Equals(a.start.x, b.start.x))
        return Equals(a.end.y, b.start.y) || Equals(a.start.y, b.end.y);

    return false;
}

Line Merge(Line a, Line b)
{
    assert((IsHorizontal(a) && IsHorizontal(b)) || (IsVertical(a) && IsVertical(b)));
    Line merged{};
    if (IsHorizontal(a))
    {
        merged.start.x = fminf(a.start.x, b.start.x);
        merged.end.x = fmaxf(a.end.x, b.end.x);

        merged.start.y = a.start.y;
        merged.end.y = a.end.y;
    }
    else if (IsVertical(a))
    {
        merged.start.y = fminf(a.start.y, b.start.y);
        merged.end.y = fmaxf(a.end.y, b.end.y);

        merged.start.x = a.start.x;
        merged.end.x = a.end.x;
    }
    return merged;
}

Line Left(Cell cell)
{
    // Top to bottom
    Line line;
    line.start.x = line.end.x = cell.col * TILE_SIZE;
    line.start.y = cell.row * TILE_SIZE;
    line.end.y = (cell.row + 1) * TILE_SIZE;
    return line;
}

Line Right(Cell cell)
{
    // Top to bottom
    Line line;
    line.start.x = line.end.x = (cell.col + 1) * TILE_SIZE;
    line.start.y = cell.row * TILE_SIZE;
    line.end.y = (cell.row + 1) * TILE_SIZE;
    return line;
}

Line Top(Cell cell)
{
    // Left to right
    Line line;
    line.start.y = line.end.y = cell.row * TILE_SIZE;
    line.start.x = cell.col * TILE_SIZE;
    line.end.x = (cell.col + 1) * TILE_SIZE;
    return line;
}

Line Bottom(Cell cell)
{
    // Left to right
    Line line;
    line.start.y = line.end.y = (cell.row + 1) * TILE_SIZE;
    line.start.x = cell.col * TILE_SIZE;
    line.end.x = (cell.col + 1) * TILE_SIZE;
    return line;
}

inline std::vector<Line> GenOutline(const std::vector<Cell>& edges, const Tiles& tiles)
{
    std::vector<Line> lines;
    for (Cell cell : edges)
    {
        // Special cases for edges of grid:
        if (IsExtent(cell))
        {
            if (cell.row == 0) lines.push_back(Top(cell));
            if (cell.col == 0) lines.push_back(Left(cell));
            if (cell.row == ROW_COUNT - 1) lines.push_back(Bottom(cell));
            if (cell.col == COL_COUNT - 1) lines.push_back(Right(cell));
            // Doesn't generate inner-lines, might need to modify to do so.
        }
        else
        {
            Cell left = { cell.row, cell.col - 1 };
            Cell right = { cell.row, cell.col + 1 };
            Cell top = { cell.row - 1, cell.col };
            Cell bottom = { cell.row + 1, cell.col };
            if (InBounds(left) && tiles[left.row][left.col] == 0) lines.push_back(Left(cell));
            if (InBounds(right) && tiles[right.row][right.col] == 0) lines.push_back(Right(cell));
            if (InBounds(top) && tiles[top.row][top.col] == 0) lines.push_back(Top(cell));
            if (InBounds(bottom) && tiles[bottom.row][bottom.col] == 0) lines.push_back(Bottom(cell));
        }
    }

    std::vector<Line> optimizedLines;
    while (!lines.empty())
    {
        Line curr = lines.back();
        lines.pop_back();

        auto condition = [&curr](Line line) { return CanMerge(curr, line); };
        auto it = std::find_if(lines.begin(), lines.end(), condition);
        while (it != lines.end())
        {
            curr = Merge(curr, *it);
            lines.erase(it);
            it = std::find_if(lines.begin(), lines.end(), condition);
        }

        optimizedLines.push_back(curr);
    }
    return optimizedLines;
}

struct Scene
{
    std::vector<std::vector<Cell>> islands;
    std::vector<std::vector<Cell>> edges;
    std::vector<std::vector<Vector2>> outlines;
};

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

    //Scene scene;
    //GenScene(scene, tiles);

    std::vector<Cell> testIsland = GenIsland({ 7, 6 }, tiles);
    std::vector<Cell> testEdges = GenEdges(testIsland, tiles);
    std::vector<Line> testOutline = GenOutline(testEdges, tiles);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Path Exit");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTiles(tiles);
        DrawTilesLines(tiles);
        //DrawScene(scene);

        for (Cell cell : testIsland)
            DrawTile(cell.row, cell.col, GREEN);

        for (Cell cell : testEdges)
            DrawTile(cell.row, cell.col, RED);

        for (Line line : testOutline)
            DrawLineEx(line.start, line.end, 5.0f, SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
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

    // Generate outlines
    std::vector<std::vector<Vector2>> outlines;
    //for (const std::vector<Cell>& edge : edges)
    //    outlines.push_back(GenOutline(edge, tiles));

    scene.islands = islands;
    scene.edges = edges;
    scene.outlines = outlines;
}

void DrawScene(const Scene& scene)
{
    for (const std::vector<Cell>& island : scene.islands)
    {
        for (Cell cell : island)
            DrawTile(cell.row, cell.col, GREEN);
    }

    for (const std::vector<Cell>& edge : scene.edges)
    {
        for (Cell cell : edge)
            DrawTile(cell.row, cell.col, RED);
    }
}
