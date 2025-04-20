#pragma once

namespace alk
{
    class Application
    {

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