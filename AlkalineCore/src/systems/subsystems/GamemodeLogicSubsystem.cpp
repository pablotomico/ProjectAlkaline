#include "raylib.h"

#include "systems/GameLogic.h"
#include "systems/subsystems/GamemodeLogicSubsystem.h"

#include "components/GamemodeLogicComponent.h"

namespace alk
{
    namespace GameLogic
    {
        bool GamemodeLogicSubsystem::Initialize()
        {
            // Initialization logic for the gamemode logic system
            SetState(EGameState::BUILD);

            alk::ScriptSystem::CreateNamespace("GamemodeLogicSubsystem")
                .AddFunction("GetState", &GamemodeLogicSubsystem::GetStateString, this);
            return true;
        }

        void GamemodeLogicSubsystem::Update()
        {
            if(IsKeyPressed(KEY_SPACE))
            {
                TransitionToNextState();
                ALK_LOG("Update: Transitioned to state: %s", GetStateString());
            }
        }

        void GamemodeLogicSubsystem::Shutdown()
        {
            ALK_LOG("Shutting down Gamemode Logic subsystem");
        }

        void GamemodeLogicSubsystem::SetState(EGameState newState)
        {
            if (currentState != newState)
            {
                EGameState oldState = currentState;
                currentState = newState;

                alk::GameLogic::Scene *activeScene = alk::GameLogic::GetActiveScene();
                if (activeScene == nullptr)
                {
                    ALK_LOG("GamemodeLogicSubsystem::SetState: Active scene is null, cannot find gamemode logic components.");
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

        EGameState GamemodeLogicSubsystem::TransitionToNextState()
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