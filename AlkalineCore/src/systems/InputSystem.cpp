#include "raylib/raylib.h"

#include "systems/InputSystem.h"
#include "systems/ScriptSystem.h"

void alk::InputSystem::Initialize()
{
    alk::ScriptSystem::RegisterNotification("OnKeyPressed");

    alk::ScriptSystem::CreateNamespace("Input")
        .AddFunction("IsKeyDown", alk::InputSystem::IsKeyDown);
}

void alk::InputSystem::Update(const float deltaTime)
{

    while (int keycode = GetKeyPressed())
    {
        alk::ScriptSystem::SendNotification("OnKeyPressed", keycode);
    }
}

bool alk::InputSystem::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}