#include "raylib.h"

#include "systems/GameLogic/GameLogic.h"
#include "systems/GameLogic/GamemodeLogicSubsystem.h"

#include "components/GamemodeLogicComponent.h"

namespace alk
{
    bool GamemodeLogicSubsystem::Initialize()
    {
        World& world = alk::GameLogic::GetWorld();
        auto components = world.GetComponents<GamemodeLogicComponent>();
        if (components->Size() == 0) return false;

        SetState(EGameState::BUILD);

        alk::ScriptSystem::CreateNamespace("GamemodeLogicSubsystem")
            .AddFunction("GetState", &GamemodeLogicSubsystem::GetStateString, this);

        return true;
    }

    void GamemodeLogicSubsystem::Update()
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            TransitionToNextState();
            ALK_LOG("Update: Transitioned to state: %s", GetStateString());
        }
    }

    void GamemodeLogicSubsystem::Reflect(ScriptSystem& script)
    {}
    
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

            alk::Scene* activeScene = alk::GameLogic::GetActiveScene();
            if (activeScene == nullptr)
            {
                ALK_LOG("GamemodeLogicSubsystem::SetState: Active scene is null, cannot find gamemode logic components.");
                return;
            }
            World& world = activeScene->GetWorld();
            auto gamemodeLogicComponents = world.GetComponents<GamemodeLogicComponent>();

            for (auto& gamemodeLogicComponent : gamemodeLogicComponents->components)
            {
                for (const auto& callback : gamemodeLogicComponent.GetStateChangeCallbacks())
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