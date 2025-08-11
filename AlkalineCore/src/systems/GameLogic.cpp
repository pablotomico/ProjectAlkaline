#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "systems/GamemodeLogicSystem.h"

std::vector<alk::GameLogic::SystemFactoryFn>& alk::GameLogic::GetFactoryList() {
    static std::vector<alk::GameLogic::SystemFactoryFn> factories;
    return factories;
}

bool alk::GameLogic::RegisterSystemFactory(alk::GameLogic::SystemFactoryFn factory)
{
    GetFactoryList().push_back(factory);
    return true;
}

void alk::GameLogic::Initialize(Scene scene)
{
    for (auto& f : GetFactoryList()) {
        AddSystem(f());
    }

    GameLogic::LoadScene(std::move(scene), true);
    
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