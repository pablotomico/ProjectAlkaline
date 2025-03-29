#include "AlkalineApplication.h"

AlkalineApplication::AlkalineApplication()
{

}
AlkalineApplication::~AlkalineApplication()
{

}
bool AlkalineApplication::Initialize()
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1600, 900, "Alkaline");
    SetTargetFPS(144);

    // ImGui
    rlImGuiSetup(true);
    // ImGui::StyleColorsDark();

    entity->AddComponent<SpriteComponent>()->SetOwner(entity);
    entity->AddComponent<TransformComponent>()->SetOwner(entity);
    if(entity->GetComponent<SpriteComponent>()->LoadSprite("assets/sprites/grass_center_N.png"))
    {
        std::cout << "Successfully loaded sprite" << std::endl;
    }

    return true;
}
void AlkalineApplication::Update(const float deltaTime)
{
}
void AlkalineApplication::FixedUpdate(const float deltaTime)
{
}
void AlkalineApplication::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawEllipse(50, 50, 20, 20, BLUE);
    entity->GetComponent<SpriteComponent>()->Draw();

    // start ImGui Conent
    rlImGuiBegin();
    // show ImGui Content
    bool open = true;
    ImGui::ShowDemoWindow(&open);
    // end ImGui Content
    rlImGuiEnd();

    EndDrawing();

}

void AlkalineApplication::Close()
{
    CloseWindow();
    rlImGuiShutdown();
}

bool AlkalineApplication::ShouldClose()
{
    return WindowShouldClose();
}
