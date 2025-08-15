#pragma once

#include "raylib.h"

#include "systems/GameLogicSubsystem.h"
#include "components/GridPreviewComponent.h"
#include "misc/GridHelpers.h"

namespace alk
{
    namespace GameLogic
    {
        class GridSubsystem : public GameLogicSubsystem
        {
            ALK_GAMELOGICSUBSYSTEM(GridSubsystem);

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