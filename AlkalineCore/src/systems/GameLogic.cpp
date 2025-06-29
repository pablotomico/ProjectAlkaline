#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "GameLogic.h"
#include "systems/GamemodeLogicSystem.h"

void alk::GameLogic::Initialize()
{
    gamemodeLogicSystem = new GamemodeLogicSystem();

    GetScenes().emplace("TestScene", Scene());
    GetActiveScene() = &GetScenes().at("TestScene");
    GetActiveScene()->Initialize();
    
    for (GameLogicSystem* system : GetSystems())
    {
        system->Initialize();
    }
}

void alk::GameLogic::Update(const float deltaTime)
{
    for (GameLogicSystem* system : GetSystems())
    {
        if (system)
        {
            system->Update();
        }
    }

    Scene* activeScene = GetActiveScene();
    if (activeScene)
    {
        activeScene->Update();
    }
}