#pragma once
#include "Tile.h"
#include "Line.h"

#include <cassert>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>

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
    std::vector<Cell> edges = island;
    edges.erase(std::remove_if(edges.begin(), edges.end(), [&tiles](Cell cell) { return !IsEdge(cell, tiles); }), edges.end());
    return edges;
}

inline std::vector<Line> GenOutline(const std::vector<Cell>& edges, const Tiles& tiles)
{
    std::vector<Line> lines;
    for (Cell cell : edges)
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
