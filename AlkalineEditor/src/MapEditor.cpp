#include "MapEditor.h"

#include "EditorUtils.h"
#include "rlImGui/rlImGui.h"
#include "imgui/imgui.h"

void MapEditor::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPIF(1280.0f), ScaleToDPIF(720.0f)), ImVec2(ScaleToDPIF(1280.0f), ScaleToDPIF(720.0f)));
    ImGui::Begin("Map Editor", &show);
    BeginTextureMode(texture);
    ClearBackground(DARKGRAY);
    BeginMode2D(camera);

    DrawGrid();

    EndMode2D();
    EndTextureMode();
    rlImGuiImageRenderTextureFit(&texture, true);
    ImGui::End();
    ImGui::PopStyleVar();
    
    DrawToolbar();
}

void MapEditor::DrawGrid()
{
    DrawCircle(0, 0, 5, PURPLE);
    int numHorizontalTiles = (int) ceil(texture.texture.width / tileSize);
    int numVerticalTiles = (int) ceil(texture.texture.height / (tileSize * 0.5f));

    for(int i = 0; i < numHorizontalTiles; i++)
    {
        for(int j = 0; j < numVerticalTiles; j++)
        {
            // DrawCircle(i * tileSize, j * tileSize * 0.5f, 2, GREEN);
        }
    }
    
}

void MapEditor::DrawToolbar()
{
    ImGui::Begin("Toolbar");


    ImGui::End();
}