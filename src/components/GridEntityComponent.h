#pragma once

#include "BaseComponent.h"
#include "include/raylib.h"
#include "alkaline_lib.h"
#include <vector>

namespace alk
{
    class GridEntityComponent : public BaseComponent {
        std::vector<Vector2> tileMap;

    public:
        GridEntityComponent() : BaseComponent() {
            ALK_LOG("Grid Entity Component Created!");
        };
        ~GridEntityComponent(){};

        void ConvertPreviewToGridMap(const std::vector<std::pair<Vector2, bool>> &validMap) {
            tileMap.clear();
            for (const auto& tile : validMap) {
                tileMap.push_back(tile.first);
            }
        };

        const std::vector<Vector2>& GetTileMap() const {
            return tileMap;
        };
    };
}