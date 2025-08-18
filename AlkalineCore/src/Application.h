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

        std::string name = "AlkalineGame";
        int width = 1920;
        int height = 1080;
        int targetFPS = 144;

    private:
        void Update(const float deltaTime);
        void FixedUpdate(const float deltaTime);
        void Draw();
        bool QueryShutdown();
    };
}