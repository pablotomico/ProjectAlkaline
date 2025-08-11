#include "GameLogicSystem.h"
#include "GameLogic.h"

namespace alk
{
    namespace GameLogic
    {
        GameLogicSystem::GameLogicSystem(std::string name) : name(name)
        {
            ALK_LOG("Registering %s", name.c_str());
            // alk::GameLogic::AddSystem(this);
        
            // Just C++ things...
            // this
            // alk::GameLogic::SubscribeToComponent<TransformComponent>([this](TransformComponent* t) { OnTransformAdded(t); });
            // or
            // alk::GameLogic::SubscribeToComponent<TransformComponent>(std::bind(&TestSystem::OnTransformAdded, this, std::placeholders::_1));
        }
    }
}
