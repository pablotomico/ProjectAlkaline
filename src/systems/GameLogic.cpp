#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"
#include "GameLogic.h"

void alk::GameLogic::Initialize()
{
    for (GameLogicSystem* system : GetSystems())
    {
        system->Initialize();
    }
}

void alk::GameLogic::Update(const float deltaTime)
{
    for (GameLogicSystem* system : GetSystems())
    {
        system->Update();
    }
}

void alk::GameLogic::Draw()
{
    for (GameLogicSystem* system : GetSystems())
    {
        system->Draw();
    }
}