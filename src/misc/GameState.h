#pragma once
#include <functional>
#include <vector>

namespace alk
{
    namespace GameLogic
    {
        enum EGameState
        {
            NONE,
            BUILD,
            BATTLE,
            REWARDS,
        };

        class GameState
        {
        private:
            EGameState currentState = EGameState::NONE;
            std::vector<std::function<void(EGameState, EGameState)>> stateChangeCallbacks;

        public:
            GameState() = default;

            EGameState TransitionToNextState()
            {
                switch (currentState)
                {
                case EGameState::BUILD:
                    SetState(EGameState::BATTLE);
                    break;
                case EGameState::BATTLE:
                    SetState(EGameState::REWARDS);
                    break;
                case EGameState::REWARDS:
                    SetState(EGameState::BUILD);
                    break;
                default:
                    SetState(EGameState::BUILD);
                    break;
                }
                return currentState;
            }

            void SetState(EGameState newState)
            {
                if (currentState != newState)
                {
                    EGameState oldState = currentState;
                    currentState = newState;

                    for (const auto& callback : stateChangeCallbacks)
                    {
                        if (callback)
                        {
                            callback(oldState, newState);
                        }
                    }
                }
            }

            EGameState GetState() const
            {
                return currentState;
            }

            const char* GetStateString() const
            {
                switch (currentState)
                {
                case EGameState::NONE:
                    return "NONE";
                case EGameState::BUILD:
                    return "BUILD";
                case EGameState::BATTLE:
                    return "BATTLE";
                case EGameState::REWARDS:
                    return "REWARDS";
                default:
                    return "UNKNOWN";
                }
            }

            void RegisterOnStateChangedCallback(const std::function<void(EGameState, EGameState)>& callback)
            {
                stateChangeCallbacks.push_back(callback);
            }
        };
    }
}
