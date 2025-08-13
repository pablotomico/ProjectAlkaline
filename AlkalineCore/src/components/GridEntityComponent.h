#pragma once

#include "BaseComponent.h"
#include "raylib.h"
#include "alkaline_lib.h"
#include <vector>

namespace alk
{
    class GridEntityComponent : public BaseComponent {
        std::vector<Vector2> tileMap;
        Vector2 gridPosition;
    public:
        GridEntityComponent() : BaseComponent() {
            ALK_LOG("Grid Entity Component Created!");
        };
        GridEntityComponent(Vector2 newGridPosition) : BaseComponent() {
            this->gridPosition = newGridPosition;
            ALK_LOG("Grid Entity Component Created!");
        };

        void ConvertPreviewToGridMap(const std::vector<std::pair<Vector2, bool>> &validMap) {
            tileMap.clear();
            for (const auto& tile : validMap) {
                tileMap.push_back(tile.first);
            }
        };

        const std::vector<Vector2>& GetTileMap() const {
            return tileMap;
        };

        inline void SetGridPosition(Vector2 newGridPosition) {
            gridPosition = newGridPosition;
        };

        inline Vector2 GetGridPosition() const {
            return gridPosition;
        };

        // TODO
        // ALK_COMPONENT_SERIALIZER(GridEntityComponent, {});
        // ALK_COMPONENT_DESERIALIZER(GridEntityComponent, {});
    };
}