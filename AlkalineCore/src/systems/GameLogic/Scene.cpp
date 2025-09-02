#include "systems/GameLogic/Scene.h"
#include "systems/GameLogic/World.h"

#include "components/GamemodeLogicComponent.h"
#include "components/TransformComponent.h"

#include "Debug/DebugUI.h"
#include "misc/GridHelpers.h"

namespace alk
{
    void Scene::Initialize()
    {
        ALK_LOG("Initializing Scene '%s'", name.c_str());

    }

    void Scene::Update()
    {

    }

    void Scene::OnGameStateChanged(EGameState oldState, EGameState newState, const char* stateString)
    {
        alk::Debug::UI::RegisterText("GameState", stateString);

        if (newState == EGameState::BUILD)
        {

        }
        else if (newState == EGameState::BATTLE)
        {

        }
        else if (newState == EGameState::REWARDS)
        {

        }
    }
}