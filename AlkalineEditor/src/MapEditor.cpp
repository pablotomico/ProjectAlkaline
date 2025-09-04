#include "MapEditor.h"

#include "EditorUtils.h"
#include "rlImGui/rlImGui.h"
#include "imgui/imgui.h"

void MapEditor::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Map Editor - View", &show, ImGuiWindowFlags_AlwaysAutoResize);
    BeginTextureMode(texture);
    ClearBackground(DARKGRAY);
    BeginMode2D(camera);

    DrawGrid();

    EndMode2D();
    EndTextureMode();
    
    rlImGuiImage(&texture.texture);
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