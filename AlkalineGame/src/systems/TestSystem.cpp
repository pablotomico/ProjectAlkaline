#include "systems/TestSystem.h"
#include "components/TestComponent.h"

bool oth::GameLogic::TestSystem::Initialize()
{
    auto& world = alk::GameLogic::GetWorld();

    auto sprites = world.GetComponents<TestComponent>();
    return true;
}

