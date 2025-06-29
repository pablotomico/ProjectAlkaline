#pragma once
#include "alkaline_lib.h"
#include "BaseComponent.h"
#include <functional>
#include <vector>
#include "systems/GamemodeLogicSystem.h"

namespace alk
{
    namespace GameLogic
    {
        using StateChangeCallback = std::function<void(EGameState, EGameState, const char*)>;

        class GamemodeLogicComponent : public BaseComponent
        {
            std::vector<StateChangeCallback> stateChangeCallbacks;
        public:
            GamemodeLogicComponent() = default;

            void RegisterOnStateChangedCallback(const StateChangeCallback& callback)
            {
                stateChangeCallbacks.push_back(callback);
            }

            std::vector<StateChangeCallback> GetStateChangeCallbacks() const
            {
                return stateChangeCallbacks;
            }
        };
    }
}