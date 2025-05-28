#include "systems/GamemodeLogicSystem.h"

#include "GameLogic.h"
#include "components/GamemodeLogicComponent.h"
#include "GamemodeLogicSystem.h"

namespace alk
{
    namespace GameLogic
    {
        void GamemodeLogicSystem::Initialize()
        {
            ALK_LOG("Initializing Gamemode Logic subsystem");
            // Initialization logic for the gamemode logic system
            SetState(EGameState::BUILD);
        }

        void GamemodeLogicSystem::Update()
        {
            if(IsKeyPressed(KEY_SPACE))
            {
                TransitionToNextState();
                ALK_LOG("Update: Transitioned to state: %s", GetStateString());
            }
        }

        void GamemodeLogicSystem::Shutdown()
        {
            ALK_LOG("Shutting down Gamemode Logic subsystem");
        }

        void GamemodeLogicSystem::SetState(EGameState newState)
        {
            if (currentState != newState)
            {
                EGameState oldState = currentState;
                currentState = newState;

                alk::GameLogic::Scene *activeScene = alk::GameLogic::GetActiveScene();
                if (activeScene == nullptr)
                {
                    ALK_LOG("GamemodeLogicSystem::SetState: Active scene is null, cannot find gamemode logic components.");
                    return;
                }
                World &world = activeScene->GetWorld();
                auto gamemodeLogicComponents = world.GetComponents<GamemodeLogicComponent>();

                for (auto &gamemodeLogicComponent : gamemodeLogicComponents->components)
                {
                    for (const auto &callback : gamemodeLogicComponent.GetStateChangeCallbacks())
                    {
                        if (callback)
                        {
                            callback(oldState, newState, GetStateString());
                        }
                    }
                }
            }
        }

        EGameState GamemodeLogicSystem::TransitionToNextState()
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
    }
}