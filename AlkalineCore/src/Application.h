#pragma once

#include "systems/CoreSystems.h"
#include "raylib/raylib.h"
#include <string>
#include <memory>

namespace alk
{
    class GameLogic;
    class ScriptSystem;
    class RenderSystem;
    class InputSystem;

    class Application
    {
    public:
        Application();
        ~Application();

        bool Initialize(const std::string& scenePath, bool initWindow = true);
        int Run();
        void Shutdown();

        std::string name = "AlkalineGame";
        int width = 1920;
        int height = 1080;
        int targetFPS = 144;

        void Update(const float deltaTime);
        void FixedUpdate(const float deltaTime);
        void Draw(RenderTexture2D* renderTexture);
        void Close();
        bool QueryShutdown();

    private:
        CoreSystems coreSystems;
        std::unique_ptr<ScriptSystem> scriptSystem;
        std::unique_ptr<GameLogic> gameLogic;
        std::unique_ptr<RenderSystem> renderSystem;
        std::unique_ptr<InputSystem> inputSystem;
    };
}