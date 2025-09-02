#pragma once
#include <functional>
#include <vector>

#include "alkaline_lib.h"
#include "components/BaseComponent.h"
#include "systems/GameLogic/GamemodeLogicSubsystem.h"
#include "serialization/SceneSerializer.h"

namespace alk
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

        ALK_COMPONENT_SERIALIZER(GamemodeLogicComponent, {});
        ALK_COMPONENT_DESERIALIZER(GamemodeLogicComponent, {});
    };
}