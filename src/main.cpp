#define SOL_ALL_SAFETIES_ON 1 // Remove whenever

#include <sstream>
#include "sol.hpp"

#include "imgui.h"
#include "rlImGui.h"

#include "include/raylib.h"

#include "alkaline_lib.h"
#include "entities/BaseEntity.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"

#define SCRIPTS_PATH "scripts/"
#define SPRITES_PATH "assets/sprites/"

/**
 * @brief lua debug functions
 *
 */
namespace Debug
{
    /**
     * @brief logs debug message to standard output
     * @param args
     */
    void Log(sol::variadic_args args)
    {
        char textBuffer[8192] = {};
        
        for (std::size_t i = 0; i < args.size(); ++i)
        {
            auto var = args[i];
            sol::type type = var.get_type();
            if (type == sol::type::string)
            {
                sprintf(textBuffer, "%s%s", textBuffer, var.as<std::string>().c_str());
            }
            else if (type == sol::type::number)
            {
                if (var.is<int>())
                {
                    sprintf(textBuffer, "%s%d", textBuffer, var.as<int>());
                }
                else if (var.is<double>())
                {
                    sprintf(textBuffer, "%s%f", textBuffer, var.as<double>());
                }
            }
            else if (type == sol::type::boolean)
            {
                if (var.as<bool>()) 
                {
                    sprintf(textBuffer, "%strue", textBuffer);
                }
                else
                {
                    sprintf(textBuffer, "%sfalse", textBuffer);
                }
            }
        }

        ALK_LOG(textBuffer);
    }
}

int main()
{
    // -------------
    // LUA EMBEDDING
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);
    lua.set_panic([](lua_State *L)
                  {
        const char* message = lua_tostring(L, -1);
        ALK_ERROR("LuaError: ", message);
        return 0; });

    // add scripts path to the package so scripts can use require
    lua["package"]["path"] = lua["package"]["path"].get<std::string>() + ";" + SCRIPTS_PATH + "?.lua";

    // Expose stuff
    lua["Debug"].get_or_create<sol::table>()
        .set_function("Log", Debug::Log);

    lua.new_usertype<BaseEntity>("BaseEntity")
        .set_function("IsValid", &BaseEntity::IsValid)
        .set_function("GetName", &BaseEntity::GetName);

    lua.set_function("BaseEntity", [](){ return new BaseEntity(); });

    // Load script
    lua.script_file("scripts/main.lua");

    lua["TestEntity"]["OnStart"]();

    // -------------

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1600, 900, "Alkaline");
    SetTargetFPS(144);

    // -------------
    // ImGui
    rlImGuiSetup(true);
    // ImGui::StyleColorsDark();

    BaseEntity* entity = new BaseEntity();
    entity->AddComponent<SpriteComponent>()->SetOwner(entity);
    entity->AddComponent<TransformComponent>()->SetOwner(entity);
    if(entity->GetComponent<SpriteComponent>()->LoadSprite("assets/sprites/grass_center_N.png"))
    {
        std::cout << "Successfully loaded sprite" << std::endl;
    }

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawEllipse(50, 50, 20, 20, BLUE);

        // start ImGui Conent
        rlImGuiBegin();

        // show ImGui Content
        bool open = true;
        ImGui::ShowDemoWindow(&open);

        entity->GetComponent<SpriteComponent>()->Draw();
        
        // end ImGui Content
        rlImGuiEnd();

        EndDrawing();

        entity->Update(1);
    }

    CloseWindow();

    rlImGuiShutdown();
    return 0;
}
