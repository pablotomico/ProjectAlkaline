#pragma once

#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"

namespace alk
{
    namespace GameLogic
    {
        class GamemodeLogicSystem : public GameLogicSystem
        {
            ALK_GAMELOGICSYSTEM(GamemodeLogicSystem);

        public:
            bool Initialize() override;
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
        private:
            EGameState currentState = EGameState::NONE;
        };
    }
}