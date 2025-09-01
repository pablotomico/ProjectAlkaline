// #include "systems/GameLogic.h"
#include "systems/GameLogic/GameLogicSubsystem.h"

namespace alk
{
    GameLogicSubsystem::GameLogicSubsystem(std::string name, CoreSystems& coreSystems) : name(name), coreSystems(coreSystems)
    {
        ALK_LOG("Registering %s", name.c_str());
        // alk::GameLogic::AddSystem(this);

        // Just C++ things...
        // this
        // alk::GameLogic::SubscribeToComponent<TransformComponent>([this](TransformComponent* t) { OnTransformAdded(t); });
        // or
        // alk::GameLogic::SubscribeToComponent<TransformComponent>(std::bind(&TestSubsystem::OnTransformAdded, this, std::placeholders::_1));
    }
}
