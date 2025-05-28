#pragma once

#include "systems/GameLogicSystem.h"
#include "include/raylib.h"

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

        class GamemodeLogicSystem : public GameLogicSystem
        {
        private:
            EGameState currentState = EGameState::NONE;

        public:
            GamemodeLogicSystem() : GameLogicSystem("Gamemode logic Subsystem") {}

            void Initialize() override;
            void Update() override;
            void Shutdown() override;


            void SetState(EGameState newState);

            const EGameState GetState() const
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

            EGameState TransitionToNextState();
        };
    }
}