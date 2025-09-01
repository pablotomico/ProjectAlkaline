#pragma once

namespace alk
{
    struct CoreSystems
    {
        class GameLogic* gameLogic = nullptr;
        class ScriptSystem* scriptSystem = nullptr;
        class RenderSystem* renderSystem = nullptr;
        class InputSystem* inputSystem = nullptr;
    };
}
