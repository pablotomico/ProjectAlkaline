#pragma once

#include "alkaline_lib.h"
#include "GameLogic.h"

namespace alk
{
    class GameLogicSystem
    {
    private:
        const char *name;

    public:
        GameLogicSystem(const char *name) : name(name)
        {
            ALK_LOG("Registering %s", name);
            alk::GameLogic::AddSystem(this);

            // Just C++ things...
            // this
            // alk::GameLogic::SubscribeToComponent<TransformComponent>([this](TransformComponent* t) { OnTransformAdded(t); });
            // or
            // alk::GameLogic::SubscribeToComponent<TransformComponent>(std::bind(&TestSystem::OnTransformAdded, this, std::placeholders::_1));
        }
        ~GameLogicSystem() {}

        virtual void Initialize() {};
        virtual void Shutdown() {};

        const char *GetName()
        {
            return name;
        }
    };
}