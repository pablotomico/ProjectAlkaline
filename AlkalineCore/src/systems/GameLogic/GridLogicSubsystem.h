#pragma once

#include "raylib.h"

#include "systems/GameLogic/GameLogicSubsystem.h"
#include "misc/GridHelpers.h"

namespace alk
{
    class GridPreviewComponent;

    class GridLogicSubsystem : public GameLogicSubsystem
    {
        ALK_GAMELOGICSUBSYSTEM(GridLogicSubsystem);

    private:
        GridHelpers::GridPointState gridState[GRID_WIDTH][GRID_HEIGHT] = { GridHelpers::GRID_POINT_EMPTY };

        void EvaluateGridPreviewPlacement(GridPreviewComponent& gridPreviewComponent);
        bool EvaluateTileValid(Vector2 gridPosition);
        Vector2 CalculateGridPreviewEntityCenterOffset(GridPreviewComponent& gridPreviewComponent);

    public:
        bool Initialize() override;
        void Reflect(ScriptSystem& script);
        void Update() override;
        void Shutdown() override;
    };
}