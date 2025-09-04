#include "MapEditor.h"

#include "EditorUtils.h"
#include "rlImGui/rlImGui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

void MapEditor::Update(const float deltaTime)
{
    const float cameraSpeed = 300.0f;

    if (IsKeyDown(KEY_A))
    {
        camera.target.x -= deltaTime * cameraSpeed;
    }
    if (IsKeyDown(KEY_D))
    {
        camera.target.x += deltaTime * cameraSpeed;
    }
    if (IsKeyDown(KEY_W))
    {
        camera.target.y -= deltaTime * cameraSpeed;
    }
    if (IsKeyDown(KEY_S))
    {
        camera.target.y += deltaTime * cameraSpeed;
    }
}

void MapEditor::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Map Editor - View", &show, ImGuiWindowFlags_AlwaysAutoResize);

    static int lastMousePosX = 0;
    static int lastMousePosY = 0;


    BeginTextureMode(texture);
    ClearBackground(DARKGRAY);
    BeginMode2D(camera);

    DrawGrid();

    DrawCircle(lastMousePosX, lastMousePosY, 2, PURPLE);

    EndMode2D();
    EndTextureMode();

    rlImGuiImage(&texture.texture);

    if (ImGui::IsItemHovered())
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mouseDelta = GetMouseDelta();
            camera.target.x -= mouseDelta.x;
            camera.target.y -= mouseDelta.y;
        }
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 contentMin = ImGui::GetWindowContentRegionMin();  // top-left inside window
            ImVec2 mouse = ImGui::GetIO().MousePos;

            lastMousePosX = (int) (mouse.x - (windowPos.x + contentMin.x));
            lastMousePosY = (int) (mouse.y - (windowPos.y + contentMin.y));
            // ALK_LOG("Mouse on texture: (%d, %d)\n", lastMousePosX, lastMousePosY);
        }

    }

    ImGui::End();
    ImGui::PopStyleVar();

    DrawToolbar();
}

void MapEditor::DrawGrid()
{
    DrawMapChunk(map);
}

void MapEditor::DrawMapChunk(const MapChunk& map)
{
    Vector2 points[2];
    float maxWidth = (float) map.width * tileSize;
    float maxHeight = (float) map.height * tileSize;

    for (float y = 0; y <= maxHeight; y += tileSize)
    {
        points[0] = Vector2{ 0, y };
        points[1] = Vector2{ maxWidth, y };
        ToIso(points, 2);
        DrawLineEx(points[0], points[1], 1.1f, WHITE);
    }
    for (float x = 0; x <= maxWidth; x += tileSize)
    {
        points[0] = Vector2{ x , 0 };
        points[1] = Vector2{ x , maxHeight };
        ToIso(points, 2);
        DrawLineEx(points[0], points[1], 1.0f, WHITE);
    }
}

void MapEditor::DrawToolbar()
{
    ImGui::Begin("Map Editor - Toolbar");


    ImGui::End();
}