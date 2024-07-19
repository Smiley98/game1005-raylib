#pragma once
#include <type_traits>
#include <array>

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
