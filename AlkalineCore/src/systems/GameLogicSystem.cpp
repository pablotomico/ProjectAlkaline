#include "GameLogicSystem.h"
#include "GameLogic.h"

namespace alk
{
    namespace GameLogic
    {
        GameLogicSystem::GameLogicSystem(const char *newName)
        {
            name = newName;
            ALK_LOG("Registering %s", name);
            // alk::GameLogic::AddSystem(this);
        
            // Just C++ things...
            // this
            // alk::GameLogic::SubscribeToComponent<TransformComponent>([this](TransformComponent* t) { OnTransformAdded(t); });
            // or
            // alk::GameLogic::SubscribeToComponent<TransformComponent>(std::bind(&TestSystem::OnTransformAdded, this, std::placeholders::_1));
        }
    }
}
