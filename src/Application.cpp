#include "Application.h"

// Libs
#include "imgui.h"
#include "rlImGui.h"
#include "include/raylib.h"
#include "alkaline_lib.h"
#include "Debug/DebugUI.h"

// Systems
#include "systems/GameLogic.h"
#include "systems/RenderSystem.h"

#include "tracy/Tracy.hpp"

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
    bool Application::Initialize()
    {
        SetTraceLogLevel(LOG_NONE);
        InitWindow(1600, 900, "Alkaline");
        SetTargetFPS(200);

        // ImGui
        rlImGuiSetup(true);

        alk::RenderSystem::Initialize();
        alk::GameLogic::Initialize();
        
        return true;
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
