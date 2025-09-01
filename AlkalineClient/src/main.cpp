#define SOL_ALL_SAFETIES_ON 1 // Remove whenever

// #include <sstream>
// #include "sol.hpp"

#include "alkaline_lib.h"
#include "Application.h"
// #include "raylib.h"

// #define SCRIPTS_PATH "scripts/"
// #define SPRITES_PATH "assets/sprites/"

/**
 * @brief lua debug functions
 *
 */
// namespace Debug
// {
//     /**
//      * @brief logs debug message to standard output
//      * @param args
//      */
//     void Log(sol::variadic_args args)
//     {
//         char textBuffer[8192] = {};
        
//         for (std::size_t i = 0; i < args.size(); ++i)
//         {
//             auto var = args[i];
//             sol::type type = var.get_type();
//             if (type == sol::type::string)
//             {
//                 sprintf_s(textBuffer, "%s%s", textBuffer, var.as<std::string>().c_str());
//             }
//             else if (type == sol::type::number)
//             {
//                 if (var.is<int>())
//                 {
//                     sprintf_s(textBuffer, "%s%d", textBuffer, var.as<int>());
//                 }
//                 else if (var.is<double>())
//                 {
//                     sprintf_s(textBuffer, "%s%f", textBuffer, var.as<double>());
//                 }
//             }
//             else if (type == sol::type::boolean)
//             {
//                 if (var.as<bool>()) 
//                 {
//                     sprintf_s(textBuffer, "%strue", textBuffer);
//                 }
//                 else
//                 {
//                     sprintf_s(textBuffer, "%sfalse", textBuffer);
//                 }
//             }
//         }

//         ALK_LOG(textBuffer);
//     }
// }



int main()
{
    // -------------
    // LUA EMBEDDING
    // sol::state lua;
    // lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);
    // lua.set_panic([](lua_State *L)
    //               {
    //     const char* message = lua_tostring(L, -1);
    //     ALK_ERROR("LuaError: ", message);
    //     return 0; });

    // // add scripts path to the package so scripts can use require
    // lua["package"]["path"] = lua["package"]["path"].get<std::string>() + ";" + SCRIPTS_PATH + "?.lua";

    // // Expose stuff
    // lua["Debug"].get_or_create<sol::table>()
    //     .set_function("Log", Debug::Log);

    // lua.new_usertype<Entity>("Entity")
    //     .set_function("IsValid", &Entity::IsValid)
    //     .set_function("GetName", &Entity::GetName);

    // lua.set_function("Entity", [](){ return new Entity(); });

    // // Load script
    // lua.script_file("scripts/main.lua");

    // lua["TestEntity"]["OnStart"]();

    // -------------
    alk::Application application;
    ALK_ASSERT(application.Initialize("C:/dev/ProjectAlkaline/AlkalineGame/scenes/test.scene"), "Critical failure during initialization, closing application");
    return application.Run();
}
