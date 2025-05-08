#pragma once
#include "alkaline_lib.h"
#include "include/raylib.h"

namespace alk
{
    namespace GameLogic
    {
        // These should never change during runtime so we can define as constexpr
        constexpr uint8_t GRID_WIDTH = 50;
        constexpr uint8_t GRID_HEIGHT = 50;
        constexpr uint8_t TILE_WIDTH = 32;
        constexpr uint8_t TILE_WIDTH_HALF = 16;
        constexpr uint8_t TILE_HEIGHT = 16;
        constexpr uint8_t TILE_HEIGHT_HALF = 8;
        constexpr Vector2 GRID_WORLD_START_POSITION = { 0, 0 };

        namespace GridHelpers
        {
            enum GridPointState
            {
                GRID_POINT_EMPTY = 0,
                GRID_POINT_FILLED = 1,
                GRID_POINT_INVALID = 2,
            };

            inline Vector2 WorldToGridPosition(Vector2 worldPosition)
            {
                Vector2 gridPosition;
                gridPosition.x = ((worldPosition.x - GRID_WORLD_START_POSITION.x) / TILE_WIDTH_HALF + (worldPosition.y - GRID_WORLD_START_POSITION.y) / TILE_HEIGHT_HALF) / 2;
                gridPosition.y = ((worldPosition.y - GRID_WORLD_START_POSITION.y) / TILE_HEIGHT_HALF - (worldPosition.x - GRID_WORLD_START_POSITION.x) / TILE_WIDTH_HALF) / 2;
                if (gridPosition.x > 0 && gridPosition.x < GRID_WIDTH && gridPosition.y > 0 && gridPosition.y < GRID_HEIGHT)
                {
                    gridPosition.x = floor(gridPosition.x);
                    gridPosition.y = floor(gridPosition.y);
                    return gridPosition;
                }
                else
                {
                    return Vector2{-1, -1};
                }
            }

            inline Vector2 GridToWorldPosition(Vector2 gridPosition)
            {
                Vector2 worldPosition;
                worldPosition.x = ((gridPosition.x - gridPosition.y) * TILE_WIDTH_HALF) + GRID_WORLD_START_POSITION.x;
                worldPosition.y = ((gridPosition.x + gridPosition.y) * TILE_HEIGHT_HALF) + GRID_WORLD_START_POSITION.y;
                return worldPosition;
            }

            inline Vector2 GetCenterGridPosition(Vector2 dimensions)
            {
                Vector2 centerGridPosition;
                centerGridPosition.x = floor(dimensions.x / 2);
                centerGridPosition.y = floor(dimensions.y / 2);
                return centerGridPosition;
            }
        }
    }
}