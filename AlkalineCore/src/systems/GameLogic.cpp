#include <map>

#include "systems/GameLogic.h"
#include "systems/subsystems/GameLogicSubsystem.h"

std::map<std::type_index, alk::GameLogic::SubsystemFactoryFn>& alk::GameLogic::GetFactoryList() {
    static std::map<std::type_index,alk::GameLogic::SubsystemFactoryFn> factories;
    return factories;
}

void alk::GameLogic::Initialize(Scene scene)
{
    for (auto& pair : GetFactoryList()) {
        auto system = pair.second();
        AddSubsystem(pair.first, system);
    }

    GameLogic::LoadScene(std::move(scene), true);
    
    for (GameLogicSubsystem* system : GetSubsystems())
    {
        bool success = system->Initialize();
        ALK_ASSERT(system->Initialize(), "GameLogic: Fail to initialize %s", system->GetName().c_str());
    }
}

void alk::GameLogic::Update(const float deltaTime)
{
    for (GameLogicSubsystem* system : GetSubsystems())
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