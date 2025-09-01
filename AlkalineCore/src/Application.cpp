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
#include "systems/Script/ScriptSystem.h"
#include "systems/GameLogic/GameLogic.h"
#include "systems/Render/RenderSystem.h"
#include "systems/Input/InputSystem.h"

#include "serialization/SceneSerializer.h"

namespace alk
{
    Application::Application() {
        scriptSystem = std::make_unique<ScriptSystem>(coreSystems);
        gameLogic = std::make_unique<GameLogic>(coreSystems);
        renderSystem = std::make_unique<RenderSystem>(coreSystems);
        inputSystem = std::make_unique<InputSystem>(coreSystems);

        coreSystems.scriptSystem = scriptSystem.get();
        coreSystems.gameLogic = gameLogic.get();
        coreSystems.renderSystem = renderSystem.get();
        coreSystems.inputSystem = inputSystem.get();
    }

    Application::~Application() {}

    /**
     * @brief Initialize the application and all dependencies
     * @return success (bool)
     */
    bool Application::Initialize(const std::string& scenePath, bool initWindow)
    {
        ALK_TRACE("ALKALINE ENGINE v0.1");
        ALK_LOG("Working Directory: '%s'", GetWorkingDirectory());

        // TODO: Move to function
        std::string gameSettingsPath = std::string(GetApplicationDirectory()) + "GameSettings.lua";
        if (FileExists(gameSettingsPath.c_str()))
        {
            sol::table gameSettings = scriptSystem->LoadTableFromFile(gameSettingsPath);
            if (gameSettings["ProjectName"].valid())
            {
                name = gameSettings["ProjectName"];
                scriptSystem->AddToPackage("/" + name + "/scripts/");
            }
            width = gameSettings["DefaultResolution"][1].valid() ? gameSettings["DefaultResolution"][1] : width;
            height = gameSettings["DefaultResolution"][2].valid() ? gameSettings["DefaultResolution"][2] : height;
            targetFPS = gameSettings["TargetFPS"].valid() ? gameSettings["TargetFPS"] : targetFPS;
        }

        if (initWindow)
        {
            SetTraceLogLevel(LOG_NONE);
            InitWindow(width, height, name.c_str());
            SetTargetFPS(targetFPS);

            // ImGui
            rlImGuiSetup(true);
        }

        // TODO: check file extension using raylib IsFileExtension
        sol::table sceneTable = scriptSystem->LoadTableFromFile(scenePath);
        ALK_ASSERT(sceneTable != sol::lua_nil, "Application::Initialize: Couldn't load scene with path: '%s'", scenePath);
        alk::Scene testScene;
        alk::SceneSerializer::DeserializeScene(testScene, sceneTable);


        // alk::SceneSerializer::DeserializeScene();
        gameLogic->Initialize(testScene);
        scriptSystem->Initialize(testScene);
        inputSystem->Initialize(testScene);
        renderSystem->Initialize(testScene);

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
            Draw(nullptr);
        }

        Shutdown();
        Close();
        return 0; // IDEA: define return values based on success, assert, exception, etc.
    }

    /**
     * @brief Main Update loop
     * @param deltaTime Elapsed time since last frame
     */
    void Application::Update(const float deltaTime)
    {
        inputSystem->Update(deltaTime);
        gameLogic->Update(deltaTime);
        scriptSystem->Update(deltaTime);

        float cameraSpeed = 300.0f;
        float cameraZoomSpeed = 10.0f;
        Camera2D* mainCamera = renderSystem->GetMainCamera();
        if (IsKeyDown(KEY_A))
        {
            mainCamera->offset.x += deltaTime * cameraSpeed;
        }
        if (IsKeyDown(KEY_D))
        {
            mainCamera->offset.x -= deltaTime * cameraSpeed;
        }
        if (IsKeyDown(KEY_W))
        {
            mainCamera->offset.y += deltaTime * cameraSpeed;
        }
        if (IsKeyDown(KEY_S))
        {
            mainCamera->offset.y -= deltaTime * cameraSpeed;
        }

        // Handles camera zoom in on mouse position
        Vector2 mouseWorldPosBeforeZoom = GetScreenToWorld2D(GetMousePosition(), *mainCamera);
        mainCamera->zoom -= (GetMouseWheelMove() * deltaTime * cameraZoomSpeed);
        Vector2 mouseWorldPosAfterZoom = GetScreenToWorld2D(GetMousePosition(), *mainCamera);
        Vector2 delta = { mouseWorldPosBeforeZoom.x - mouseWorldPosAfterZoom.x, mouseWorldPosBeforeZoom.y - mouseWorldPosAfterZoom.y };
        mainCamera->target = Vector2{ mainCamera->target.x + delta.x, mainCamera->target.y + delta.y };
    }

    /**
     * @brief Fixed Update loop capped at 60fps
     * @param deltaTime Elapsed time since last frame
     */
    void Application::FixedUpdate(const float deltaTime)
    {}

    /**
     * @brief Draw loop for application
     */
    void Application::Draw(RenderTexture2D* renderTexture)
    {
        if (renderTexture)
        {
            BeginTextureMode(*renderTexture);
        }
        else
        {
            BeginDrawing();
            rlImGuiBegin();
        }
        ClearBackground(PINK);

        renderSystem->Draw();
        alk::Debug::UI::Draw();

        if (renderTexture)
        {
            EndTextureMode();
        }
        else
        {
            rlImGuiEnd();
            EndDrawing();
        }
        FrameMark;
    }

    /**
     * @brief Close window and clean up
     */
    void Application::Shutdown()
    {
        gameLogic->Shutdown();
        inputSystem->Shutdown();
        renderSystem->Shutdown();
        scriptSystem->Shutdown();
    }

    void Application::Close()
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
