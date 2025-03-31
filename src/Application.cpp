#include "Application.h"
#include "imgui.h"
#include "rlImGui.h"
#include "include/raylib.h"
#include "entities/BaseEntity.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"
#include "alkaline_lib.h"

namespace alk
{
    Application::Application()
    {
        entity = new BaseEntity();
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
        // ImGui::StyleColorsDark();

        alk::GameLogic::Initialize();

        entity->AddComponent<SpriteComponent>()->SetOwner(entity);
        entity->AddComponent<TransformComponent>()->SetOwner(entity);
        if(entity->GetComponent<SpriteComponent>()->LoadSprite("assets/sprites/grass_center_N.png"))
        {
            ALK_SUCCESS("Successfully loaded sprite");
        }

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

        // DrawEllipse(50, 50, 20, 20, BLUE);
        // entity->GetComponent<SpriteComponent>()->Draw();

        // start ImGui Conent
        rlImGuiBegin();
        // show ImGui Content
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        // end ImGui Content
        rlImGuiEnd();

        alk::GameLogic::Draw();

        EndDrawing();
    }

    /**
     * @brief Close window and clean up
     */
    void Application::Shutdown()
    {
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
