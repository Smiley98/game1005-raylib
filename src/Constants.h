#pragma once
constexpr float SCREEN_WIDTH = 960.0f;
constexpr float SCREEN_HEIGHT = 640.0f;

constexpr float ASPECT = SCREEN_WIDTH / SCREEN_HEIGHT;
constexpr float TILE_SIZE = 32.0f;

constexpr int ROW_COUNT = SCREEN_HEIGHT / TILE_SIZE;
constexpr int COL_COUNT = SCREEN_WIDTH / TILE_SIZE;