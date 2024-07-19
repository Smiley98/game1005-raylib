#pragma once
#include "Constants.h"
#include <raylib.h>
#include <array>

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
