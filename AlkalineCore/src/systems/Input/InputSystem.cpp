#include "raylib/raylib.h"

#include "systems/Input/InputSystem.h"
#include "systems/Script/ScriptSystem.h"

alk::InputSystem::InputSystem(CoreSystems& coreSystems) : BaseSystem(coreSystems) {}

void alk::InputSystem::Initialize(Scene& scene)
{
    std::string settingsPath = std::string(GetApplicationDirectory()) + "InputSettings.lua";
    sol::table settings = coreSystems.scriptSystem->LoadTableFromFile(settingsPath);

    sol::table inputContextTable = settings["InputContexts"];
    sol::table inputActionTable = settings["InputActions"];

    // IDEA: input action list
    // we don't store a list of input actions which are referenced from the input contexts
    // instead, the actions are defined on each context's actionMap, so can be duplicated if reused

    auto& inputContexts = GetInputContexts();

    for (auto& pair : inputContextTable)
    {
        InputContext context;
        context.name = pair.first.as<std::string>();
        sol::table actions = pair.second.as<sol::table>()["actions"];
        for (auto& p : actions)
        {
            InputAction inputAction;
            inputAction.name = p.second.as<std::string>();
            inputAction.key = inputActionTable[inputAction.name]["key"];
            context.actionMap[inputAction.key] = inputAction;
        }

        inputContexts[context.name] = context;
    }

    coreSystems.scriptSystem->RegisterNotification("OnKeyPressed");

    coreSystems.scriptSystem->CreateNamespace("Input")
        .AddFunction("LoadInputContext", &alk::InputSystem::LoadInputContext, this)
        .AddFunction("IsKeyDown", &alk::InputSystem::IsKeyDown, this);
}

void alk::InputSystem::Reflect(ScriptSystem& script)
{}

void alk::InputSystem::Update(const float deltaTime)
{
    auto context = GetActiveInputContext();
    while (int keycode = GetKeyPressed())
    {
        if (context && context->actionMap.contains(keycode))
        {
            coreSystems.scriptSystem->SendNotification("OnKeyPressed", context->actionMap.at(keycode).name);
        }
    }
}

void alk::InputSystem::Shutdown()
{}

bool alk::InputSystem::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

void alk::InputSystem::LoadInputContext(const std::string& name)
{
    auto& contexts = GetInputContexts();
    ALK_ASSERT(contexts.contains(name), "[InputSystem] Input Context '%s' doesn't exist", name.c_str());
    currentContext = &contexts.at(name);
}

alk::InputContext*& alk::InputSystem::GetActiveInputContext()
{
    return currentContext;
}

alk::InputContextMap& alk::InputSystem::GetInputContexts()
{
    return inputContexts;
}
