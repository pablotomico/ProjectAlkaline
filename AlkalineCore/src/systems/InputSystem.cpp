#include "raylib/raylib.h"

#include "systems/InputSystem.h"
#include "systems/ScriptSystem.h"
#include "systems/input/InputContext.h"
#include "InputSystem.h"


void alk::InputSystem::Initialize()
{
    std::string settingsPath = std::string(GetApplicationDirectory()) + "InputSettings.lua";
    sol::table settings = alk::ScriptSystem::LoadTableFromFile(settingsPath);

    sol::table inputContextTable = settings["InputContexts"];
    sol::table inputActionTable = settings["InputActions"];
    
    // IDEA: input action list
    // we don't store a list of input actions which are referenced from the input contexts
    // instead, the actions are defined on each context's actionMap, so can be duplicated if reused
    
    auto& inputContexts = GetInputContexts();
    
    for(auto& pair : inputContextTable)
    {
        InputContext context;
        context.name = pair.first.as<std::string>();
        sol::table actions = pair.second.as<sol::table>()["actions"];
        for(auto& p : actions)
        {
            InputAction inputAction;
            inputAction.name = p.second.as<std::string>();
            inputAction.key = inputActionTable[inputAction.name]["key"];
            context.actionMap[inputAction.key] = inputAction;
        }

        inputContexts[context.name] = context;
    }

    alk::ScriptSystem::RegisterNotification("OnKeyPressed");

    alk::ScriptSystem::CreateNamespace("Input")
        .AddFunction("LoadInputContext", alk::InputSystem::LoadInputContext)
        .AddFunction("IsKeyDown", alk::InputSystem::IsKeyDown);
}

void alk::InputSystem::Update(const float deltaTime)
{
    auto context = GetActiveInputContext();
    while (int keycode = GetKeyPressed())
    {
        if (context && context->actionMap.contains(keycode))
        {
            alk::ScriptSystem::SendNotification("OnKeyPressed", context->actionMap.at(keycode).name);
        }
    }
}

bool alk::InputSystem::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

void alk::InputSystem::LoadInputContext(const std::string& name)
{
    auto& contexts = alk::InputSystem::GetInputContexts();
    ALK_ASSERT(contexts.contains(name), "[InputSystem] Input Context '%s' doesn't exist", name.c_str());
    GetActiveInputContext() = &contexts.at(name);
}

alk::InputSystem::InputContext*& alk::InputSystem::GetActiveInputContext()
{
    static alk::InputSystem::InputContext* currentContext = nullptr;
    return currentContext;
}

alk::InputSystem::InputContextMap& alk::InputSystem::GetInputContexts()
{
    static alk::InputSystem::InputContextMap inputContexts;
    return inputContexts;
}
