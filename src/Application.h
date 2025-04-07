#pragma once

#include "systems/Scene.h"

class BaseEntity; // Forward Declaration

namespace alk
{
    class Application
    {
    private:
        BaseEntity* entity;

        alk::GameLogic::Scene scene;

    public:
        Application();
        ~Application();
    
        bool Initialize();
        void Update(const float deltaTime);
        void FixedUpdate(const float deltaTime);
        void Draw();
        void Shutdown();
        bool QueryShutdown();
    };
}