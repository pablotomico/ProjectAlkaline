#pragma once

#include <string>

namespace alk
{
    class Application
    {
    public:
        Application();
        ~Application();
    
        bool Initialize(const std::string& scenePath);
        int Run();
        void Shutdown();
        
    private:
        void Update(const float deltaTime);
        void FixedUpdate(const float deltaTime);
        void Draw();
        bool QueryShutdown();
    };
}