#pragma once

#include "systems/GameLogicSystem.h"
#include "raylib.h"
#include "components/GridPreviewComponent.h"
#include "misc/GridHelpers.h"

namespace alk
{
    namespace GameLogic
    {
        class GridSystem : public GameLogicSystem
        {
        private:
            std::vector<Vector2> positionArray;
            GridHelpers::GridPointState gridState[GRID_WIDTH][GRID_HEIGHT] = {GridHelpers::GRID_POINT_EMPTY};

            void EvaluateGridPreviewPlacement(GridPreviewComponent &gridPreviewComponent);
            bool EvaluateTileValid(Vector2 gridPosition);
            Vector2 CalculateGridPreviewEntityCenterOffset(GridPreviewComponent &gridPreviewComponent);

        public:
            GridSystem() : GameLogicSystem("Grid Subsystem") {}

            void Initialize() override;
            void Update() override;
            void Shutdown() override;

            const std::vector<Vector2>& GetGridArray() const { return positionArray; };
        };
    }
}