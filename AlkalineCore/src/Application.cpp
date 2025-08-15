#include "Application.h"

#include <memory>

// Libs
#include "imgui.h"
#include "rlImGui.h"
#include "raylib.h"
#include "alkaline_lib.h"
#include "Debug/DebugUI.h"
#include "Tracy.hpp"

// Systems
#include "systems/ScriptSystem.h"
#include "systems/GameLogic.h"
#include "systems/RenderSystem.h"

#include "serialization/SceneSerializer.h"

namespace alk
{
    

    Application::Application()
    {
    }
    Application::~Application()
    {

    }

    /**
     * @brief Initialize the application and all dependencies
     * @return success (bool)
     */
    bool Application::Initialize(const std::string& scenePath)
    {
        ALK_TRACE("ALKALINE ENGINE v0.1");
        ALK_LOG("Working Directory: '%s'", GetWorkingDirectory());
        alk::ScriptSystem::Initialize(); // TODO: Investigate if loading internal things might expose them to the scripts as globals
        
        // TODO: Move to function
        std::string gameSettingsPath = std::string(GetApplicationDirectory()) + "GameSettings.lua";
        if (FileExists(gameSettingsPath.c_str()))
        {
            sol::table gameSettings = alk::ScriptSystem::LoadTableFromFile(gameSettingsPath);
            if (gameSettings["ProjectName"].valid())
            {
                name = gameSettings["ProjectName"];
                alk::ScriptSystem::AddToPackage("/" + name + "/scripts/");
            }
            width = gameSettings["DefaultResolution"][1].valid() ? gameSettings["DefaultResolution"][1] : width;
            height = gameSettings["DefaultResolution"][2].valid() ? gameSettings["DefaultResolution"][2] : height;
            targetFPS = gameSettings["TargetFPS"].valid() ? gameSettings["TargetFPS"] : targetFPS;
        }
        
        SetTraceLogLevel(LOG_NONE);
        InitWindow(width, height, name.c_str());
        SetTargetFPS(targetFPS);

        // ImGui
        rlImGuiSetup(true);

        // TODO: check file extension using raylib IsFileExtension
        sol::table sceneTable = alk::ScriptSystem::LoadTableFromFile(scenePath);
        ALK_ASSERT(sceneTable != sol::lua_nil, "Application::Initialize: Couldn't load scene with path: '%s'", scenePath);
        alk::GameLogic::Scene testScene;
        alk::SceneSerializer::DeserializeScene(testScene, sceneTable);


        // alk::SceneSerializer::DeserializeScene();
        alk::GameLogic::Initialize(std::move(testScene));
        alk::RenderSystem::Initialize();
        
        ALK_TRACE("APPLICATION INITIALIZED SUCCESSFULLY");
        return true;
    }

    int Application::Run()
    {
        double const fixedTimeStep = 1 / fixedUpdateFPS;
        double nextFixedUpdate = 0;

        while (!QueryShutdown()) // Detect window close button or ESC key
        {
            float deltaTime = GetFrameTime();
            Update(deltaTime);

            double currentTime = GetTime();
            if (currentTime > nextFixedUpdate)
            {
                FixedUpdate(fixedTimeStep);
                nextFixedUpdate = currentTime + fixedTimeStep;
            }
            Draw();
        }

        Shutdown();
        return 0; // IDEA: define return values based on success, assert, exception, etc.
    }

    /**
     * @brief Main Update loop
     * @param deltaTime Elapsed time since last frame
     */
    void Application::Update(const float deltaTime)
    {
        alk::GameLogic::Update(deltaTime);

        float cameraSpeed = 300.0f;
        float cameraZoomSpeed = 10.0f;
        Camera2D& mainCamera = RenderSystem::GetMainCamera();
        if(IsKeyDown(KEY_A))
        {
            mainCamera.offset.x += deltaTime * cameraSpeed;
        }
        if(IsKeyDown(KEY_D))
        {
            mainCamera.offset.x -= deltaTime * cameraSpeed;
        }
        if(IsKeyDown(KEY_W))
        {
            mainCamera.offset.y += deltaTime * cameraSpeed;
        }
        if(IsKeyDown(KEY_S))
        {
            mainCamera.offset.y -= deltaTime * cameraSpeed;
        }

        // Handles camera zoom in on mouse position
        Vector2 mouseWorldPosBeforeZoom = GetScreenToWorld2D(GetMousePosition(), mainCamera);
        mainCamera.zoom -= (GetMouseWheelMove() * deltaTime * cameraZoomSpeed);
        Vector2 mouseWorldPosAfterZoom = GetScreenToWorld2D(GetMousePosition(), mainCamera);
        Vector2 delta = { mouseWorldPosBeforeZoom.x - mouseWorldPosAfterZoom.x, mouseWorldPosBeforeZoom.y - mouseWorldPosAfterZoom.y };
        mainCamera.target = Vector2{mainCamera.target.x + delta.x, mainCamera.target.y + delta.y };   
    }

    /**
     * @brief Fixed Update loop capped at 60fps
     * @param deltaTime Elapsed time since last frame
     */
    void Application::FixedUpdate(const float deltaTime)
    {
    }

    /**
     * @brief Draw loop for application
     */
    void Application::Draw()
    {
        BeginDrawing();
        ClearBackground(PINK);

        alk::RenderSystem::Draw();
        alk::Debug::UI::Draw();

        EndDrawing();
        FrameMark;
    }

    /**
     * @brief Close window and clean up
     */
    void Application::Shutdown()
    {
        alk::RenderSystem::Shutdown();
        CloseWindow();
        rlImGuiShutdown();
    }

    /**
     * @brief returns whether the application should close
     * @return bool 
     */
    bool Application::QueryShutdown()
    {
        return WindowShouldClose();
    }
}
