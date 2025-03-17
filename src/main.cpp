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

    Actor() : id(0), name("None") {}

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

    /**
     * @brief Returns the actor name - debug purposes only
     * 
     * @return const char* 
     */
    const char * GetName()
    {
        return name;
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

    lua.new_usertype<Actor>("Actor")
        .set_function("IsValid", &Actor::IsValid)
        .set_function("GetName", &Actor::GetName);

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
