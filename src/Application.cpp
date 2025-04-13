#include "Application.h"

// Libs
#include "imgui.h"
#include "rlImGui.h"
#include "include/raylib.h"
#include "alkaline_lib.h"

// Systems
#include "systems/RenderSystem.h"

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
        SetTargetFPS(144);

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

        ClearBackground(BLACK);

        // start ImGui Conent
        rlImGuiBegin();

        ImGui::Begin("Performance");
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::End();

        // end ImGui Content
        rlImGuiEnd();

        //alk::GameLogic::Draw();
        alk::RenderSystem::Draw();

        EndDrawing();
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
