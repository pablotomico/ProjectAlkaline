#pragma once
#include "systems/GamemodeLogicSystem.h"

namespace alk
{
    class Application
    {
        alk::GameLogic::GamemodeLogicSystem gamemodeLogicSystem;

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