#pragma once
#include "Constants.h"
#include "Math.h"
#include "Cell.h"

struct Line
{
    Vector2 start;
    Vector2 end;
};

inline bool IsHorizontal(Line line)
{
    return Equals(line.start.y, line.end.y);
}

inline bool IsVertical(Line line)
{
    return Equals(line.start.x, line.end.x);
}

inline bool CanMerge(Line a, Line b)
{
    // If horizontal and same row
    if (IsHorizontal(a) && IsHorizontal(b) && Equals(a.start.y, b.start.y))
        return Equals(a.end.x, b.start.x) || Equals(a.start.x, b.end.x);

    // If vertical and same column
    if (IsVertical(a) && IsVertical(b) && Equals(a.start.x, b.start.x))
        return Equals(a.end.y, b.start.y) || Equals(a.start.y, b.end.y);

    return false;
}

inline Line Merge(Line a, Line b)
{
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

inline Line Left(Cell cell)
{
    // Top to bottom
    Line line;
    line.start.x = line.end.x = cell.col * TILE_SIZE;
    line.start.y = cell.row * TILE_SIZE;
    line.end.y = (cell.row + 1) * TILE_SIZE;
    return line;
}

inline Line Right(Cell cell)
{
    // Top to bottom
    Line line;
    line.start.x = line.end.x = (cell.col + 1) * TILE_SIZE;
    line.start.y = cell.row * TILE_SIZE;
    line.end.y = (cell.row + 1) * TILE_SIZE;
    return line;
}

inline Line Top(Cell cell)
{
    // Left to right
    Line line;
    line.start.y = line.end.y = cell.row * TILE_SIZE;
    line.start.x = cell.col * TILE_SIZE;
    line.end.x = (cell.col + 1) * TILE_SIZE;
    return line;
}

inline Line Bottom(Cell cell)
{
    // Left to right
    Line line;
    line.start.y = line.end.y = (cell.row + 1) * TILE_SIZE;
    line.start.x = cell.col * TILE_SIZE;
    line.end.x = (cell.col + 1) * TILE_SIZE;
    return line;
}