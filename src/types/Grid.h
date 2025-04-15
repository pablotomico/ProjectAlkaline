#pragma once

#include "include/raylib.h"
#include "entities/BaseEntity.h"
#include "alkaline_lib.h"

#include "components/RenderComponent.h"
#include "components/TransformComponent.h"

namespace alk
{
    struct Grid : public BaseEntity
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
            AddComponent<TransformComponent>(width * height);

            std::vector<Vector2>& positionArray = GetComponent<TransformComponent>()->GetPositionArray();
            for (int i = 0; i < gridWidth; ++i)
            {
                for (int j = 0; j < gridHeight; ++j)
                {
                    auto startPosX = gridScreenPosition.x + tileWidthHalf * j - tileWidthHalf * i;
                    auto startPosY = gridScreenPosition.y + tileHeightHalf * j + tileHeightHalf * i;
                    positionArray.emplace_back(Vector2{startPosX, startPosY});
                }
            }

            AddComponent<RenderComponent>(RenderSystem::RenderType::Grid, tileWidthHalf, tileHeightHalf);
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

        Vector2 GridToScreenPosition(Vector2 gridPosition)
        {
            Vector2 screenPosition;
            screenPosition.x = ((gridPosition.x - gridPosition.y) * tileWidthHalf) + gridScreenPosition.x;
            screenPosition.y = ((gridPosition.x + gridPosition.y) * tileHeightHalf) + gridScreenPosition.y;
            return screenPosition;
        }
    };
}