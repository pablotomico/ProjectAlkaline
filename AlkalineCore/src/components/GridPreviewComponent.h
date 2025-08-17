#pragma once

#include "BaseComponent.h"
#include "raylib.h"

namespace alk
{
    namespace GameLogic
    {

        class GridPreviewComponent : public BaseComponent
        {
            Vector2 dimensions = { 0, 0 };
            Vector2 gridPosition = { 0, 0 };
            std::vector<std::pair<Vector2, bool>> validMap;

        public:
            GridPreviewComponent(Vector2 dimensions) : BaseComponent()
            {
                this->dimensions = dimensions;
                for (int i = 0; i < dimensions.x; ++i)
                {
                    for (int j = 0; j < dimensions.y; ++j)
                    {
                        Vector2 gridPosition = { static_cast<float>(i), static_cast<float>(j) };
                        validMap.emplace_back(std::make_pair(gridPosition, false));
                    }
                }

                ALK_LOG("Grid Preview Component Created!");
            };
            ~GridPreviewComponent() {};

            const Vector2 GetGridPosition() const
            {
                return gridPosition;
            };
            void SetGridPosition(Vector2 gridPosition)
            {
                this->gridPosition = gridPosition;
            };
            const Vector2 GetDimensions() const
            {
                return dimensions;
            }
            std::vector<std::pair<Vector2, bool>>& GetValidMap()
            {
                return validMap;
            };
            const bool QueryValidPlacement() const
            {
                for (const auto& tile : validMap)
                {
                    if (!tile.second)
                    {
                        return false;
                    }
                }
                return true;
            };
        };
    }
}