#include "systems/GameLogic.h"
#include "systems/subsystems/GameLogicSubsystem.h"

namespace alk
{
    namespace GameLogic
    {
        GameLogicSubsystem::GameLogicSubsystem(std::string name) : name(name)
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
}
