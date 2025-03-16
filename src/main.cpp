#define SOL_ALL_SAFETIES_ON 1 // Remove whenever

#include <sstream>
#include "sol.hpp"

#include "imgui.h"
#include "rlImGui.h"

#include "include/raylib.h"

#include "alkaline_lib.h"

#define SCRIPTS_PATH "scripts/"
#define SPRITES_PATH "assets/sprites/"

/**
 * @brief Actor
 */
struct Actor
{
    /**
     * @brief unique identifier
     * 
     * @todo generate automatically via register
     */
    unsigned short id;

    /**
     * @brief Readable name of the actor (dev-only)
     * 
     * @todo generate base on class + id (?)
     */
    const char *name;

    Actor() : id(0) {}

    /**
     * @brief Checks if it's a valid actor reference
     *
     * @return true
     * @return false
     */
    bool IsValid()
    {
        return id > 0;
    }
};

/**
 * @brief lua debug functions
 *
 */
namespace Debug
{
    /**
     * @brief logs debug message to standard output
     * @param msg
     * @param args
     * 
     * @todo process args to build string
     */
    void Log(const char *msg, sol::variadic_args args)
    {
        auto lua_args = sol::as_args(args);
        ALK_LOG(msg, lua_args);
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

    sol::usertype<Actor> actor_type = lua.new_usertype<Actor>("Actor");
    actor_type.set_function("IsValid", &Actor::IsValid);
    lua.set_function("Actor", [](){ return new Actor(); });

    // Load script
    lua.script_file("scripts/main.lua");

    lua["TestEntity"]["OnStart"]();

    // -------------

    InitWindow(1920, 1080, "Alkaline");
    SetTargetFPS(144);

    // -------------
    // ImGui
    rlImGuiSetup(true);
    // ImGui::StyleColorsDark();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // start ImGui Conent
        rlImGuiBegin();

        // show ImGui Content
        bool open = true;
        ImGui::ShowDemoWindow(&open);

        // end ImGui Content
        rlImGuiEnd();

        EndDrawing();
    }

    CloseWindow();

    rlImGuiShutdown();
    return 0;
}
