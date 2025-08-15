#include "systems/TestSubsystem.h"
#include "components/TestComponent.h"

bool oth::GameLogic::TestSubsystem::Initialize()
{
    auto& world = alk::GameLogic::GetWorld();

    auto test = world.GetComponents<TestComponent>();
    return true;
}

