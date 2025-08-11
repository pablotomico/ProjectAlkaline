#include <map>

#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "systems/GamemodeLogicSystem.h"

std::map<std::type_index, alk::GameLogic::SystemFactoryFn>& alk::GameLogic::GetFactoryList() {
    static std::map<std::type_index,alk::GameLogic::SystemFactoryFn> factories;
    return factories;
}

void alk::GameLogic::Initialize(Scene scene)
{
    for (auto& pair : GetFactoryList()) {
        auto system = pair.second();
        AddSystem(pair.first, system);
    }

    GameLogic::LoadScene(std::move(scene), true);
    
    for (GameLogicSystem* system : GetSystems())
    {
        bool success = system->Initialize();
        if(success)
            ALK_TRACE("GameLogic: Initialized %s", system->GetName().c_str());
        else
            ALK_ERROR("GameLogic: Fail to initialize %s", system->GetName().c_str());
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