#pragma once

#include "raylib.h"

#include "systems/GameLogicSystem.h"
#include "components/GridPreviewComponent.h"
#include "misc/GridHelpers.h"

namespace alk
{
    namespace GameLogic
    {
        class GridSystem : public GameLogicSystem
        {
            ALK_GAMELOGICSYSTEM(GridSystem);

        private:
            GridHelpers::GridPointState gridState[GRID_WIDTH][GRID_HEIGHT] = {GridHelpers::GRID_POINT_EMPTY};

            void EvaluateGridPreviewPlacement(GridPreviewComponent &gridPreviewComponent);
            bool EvaluateTileValid(Vector2 gridPosition);
            Vector2 CalculateGridPreviewEntityCenterOffset(GridPreviewComponent &gridPreviewComponent);

        public:
            bool Initialize() override;
            void Update() override;
            void Shutdown() override;
        };
    }
}