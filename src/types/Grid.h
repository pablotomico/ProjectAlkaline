#pragma once

#include "include/raylib.h"

#include "alkaline_lib.h"

namespace alk
{
    struct Grid
    {
        uint gridWidth;
        uint gridHeight;

        uint tileWidth;
        uint tileWidthHalf;

        uint tileHeight;
        uint tileHeightHalf;

        Vector2 gridScreenPosition;

        Grid(uint width, uint height, uint tileSize, Vector2 position) : gridWidth(width), gridHeight(height),
                                                                         tileWidth(tileSize), tileWidthHalf(tileSize / 2),
                                                                         tileHeight(tileSize / 2), tileHeightHalf(tileSize / 4),
                                                                         gridScreenPosition(position)
        {
            for (int i = 0; i < gridWidth; ++i)
            {
                for (int j = 0; j < gridHeight; ++j)
                {
                    auto startPosX = gridScreenPosition.x + tileWidthHalf * j - tileWidthHalf * i;
                    auto startPosY = gridScreenPosition.y + tileHeightHalf * j + tileHeightHalf * i;
                    points.push_back(Vector2{startPosX, startPosY});
                }
            }
        }

        void Draw()
        {
            for (Vector2 point : points)
            {
                auto endPosX = point.x + tileWidthHalf;
                auto endPosY = point.y + tileHeightHalf;
                DrawLine(point.x, point.y, endPosX, endPosY, WHITE);
                DrawLine(point.x - tileWidthHalf, point.y + tileHeightHalf, endPosX - tileWidthHalf, endPosY + tileHeightHalf, WHITE);

                DrawLine(point.x, point.y, point.x - tileWidthHalf, point.y + tileHeightHalf, WHITE);
                DrawLine(endPosX, endPosY, endPosX - tileWidthHalf, endPosY + tileHeightHalf, WHITE);
            }
        }

        Vector2 ScreenToGridPosition(Vector2 screen)
        {
            Vector2 gridPosition;
            gridPosition.x = ((screen.x - gridScreenPosition.x) / tileWidthHalf + (screen.y - gridScreenPosition.y) / tileHeightHalf) / 2;
            gridPosition.y = ((screen.y - gridScreenPosition.y) / tileHeightHalf - (screen.x - gridScreenPosition.x) / tileWidthHalf) / 2;
            if (gridPosition.x > 0 && gridPosition.x < gridWidth && gridPosition.y > 0 && gridPosition.y < gridHeight)
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

    private:
        std::vector<Vector2> points;
    };
}