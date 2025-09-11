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
    static Vector2 mouseWorldPos{ 0, 0 };
    static Vector2i tilePressed{ -1, -1 };
    static Vector2 points[6];

    BeginTextureMode(texture);
    ClearBackground(BLANK);
    BeginMode2D(camera);

    DrawGrid();

    //ALK_LOG("MouseWorldPos: (%d, %d)(%f, %f)\n", lastMousePosX, lastMousePosY, mouseWorldPos.x, mouseWorldPos.y);
    //DrawCircleV(mouseWorldPos, 2, GREEN);

    if (tilePressed.x >= 0 && tilePressed.y >= 0 && tilePressed.x < map.width && tilePressed.y < map.height)
    {
        DrawLineStrip(points, 5, BEIGE);

        if (spritesheet.tileSelected.x >= 0 && spritesheet.tileSelected.y >= 0 && spritesheet.tileSelected.x < spritesheet.columns && spritesheet.tileSelected.y < spritesheet.rows) {
            
            DrawTextureRec(spritesheet.texture, spritesheet.rec, points[5], WHITE);
        }
    }
    
    EndMode2D();
    //DrawCircle(lastMousePosX, lastMousePosY, 2, PURPLE);
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
            if (tilePressed.x >= 0 && tilePressed.y >= 0 && tilePressed.x < map.width && tilePressed.y < map.height) 
            {
                if (spritesheet.tileSelected.x >= 0 && spritesheet.tileSelected.y >= 0 && spritesheet.tileSelected.x < spritesheet.columns && spritesheet.tileSelected.y < spritesheet.rows) 
                {
                    map[tilePressed.x][tilePressed.y] = spritesheet.tileSelected.x + spritesheet.tileSelected.y * spritesheet.columns;
                }
            }
        }

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();  // top-left inside window
        ImVec2 mouse = ImGui::GetIO().MousePos;

        lastMousePosX = (int) (mouse.x - (windowPos.x + contentMin.x));
        lastMousePosY = (int) (mouse.y - (windowPos.y + contentMin.y));
        mouseWorldPos = GetScreenToWorld2D({ (float)lastMousePosX, (float)lastMousePosY }, camera);
        Vector2 test = ToCart(mouseWorldPos);
        tilePressed = ToMapCoord(test, tileSize);
        points[0] = { (float) tilePressed.x * tileSize, (float)tilePressed.y * tileSize };
        points[1] = { (float)(tilePressed.x + 1) * tileSize, (float)tilePressed.y * tileSize };
        points[2] = { (float)(tilePressed.x + 1) * tileSize, (float)(tilePressed.y + 1) * tileSize };
        points[3] = { (float)tilePressed.x * tileSize, (float)(tilePressed.y + 1) * tileSize };
        points[4] = { (float) tilePressed.x * tileSize, (float)tilePressed.y * tileSize };
        points[5] = { (float)(tilePressed.x - 1.5f) * tileSize, (float)(tilePressed.y - 0.5f) * tileSize };
        ToIso(points, 6);
        //ALK_LOG("Mouse on texture: (%d, %d)\n", coord.x, coord.y);

    }

    ImGui::End();
    ImGui::PopStyleVar();

    DrawToolbar();
}

void MapEditor::DrawGrid()
{
    DrawMapChunk(map);
}

void MapEditor::DrawMapChunk(MapChunk& map) const
{
    Vector2 points[2];
    float maxWidth = (float) map.width * tileSize;
    float maxHeight = (float) map.height * tileSize;

    for (float y = 0; y <= maxHeight; y += tileSize)
    {
        points[0] = Vector2{ 0, y };
        points[1] = Vector2{ maxWidth, y };
        ToIso(points, 2);
        DrawLineEx(points[0], points[1], 1.0f, WHITE);
    }
    for (float x = 0; x <= maxWidth; x += tileSize)
    {
        points[0] = Vector2{ x , 0 };
        points[1] = Vector2{ x , maxHeight };
        ToIso(points, 2);
        DrawLineEx(points[0], points[1], 1.0f, WHITE);
    }

    for(size_t x = 0; x < map.width; x++)
    {
        for (size_t y = 0; y < map.height; y++)
        {
            if(map[x][y] >= 0)
            {
                Vector2 pos = { ((float)x - 1.5f) * tileSize, ((float)y - 0.5f) * tileSize };
                auto rec = spritesheet[map[x][y]];
                auto isoPos = ToIso(pos);
                //isoPos.x -= tileSize;
                //isoPos.y -= tileSize; // TODO: investigate where the problem with this is
                DrawTextureRec(spritesheet.texture, rec, isoPos, WHITE);
            }
        }
    }

}

void MapEditor::DrawToolbar()
{
    ImGui::Begin("Map Editor - Toolbar");

    BeginTextureMode(spritesheet.renderTexture);
    ClearBackground(BLANK);


    DrawTexture(spritesheet.texture, 0, 0, WHITE);

    ImGui::SliderInt("Tile Size", &spritesheet.tileSize, 24, 256, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Columns", &spritesheet.columns, 1, spritesheet.texture.width / spritesheet.tileSize, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderInt("Rows", &spritesheet.rows, 1, spritesheet.texture.height / spritesheet.tileSize, "%d", ImGuiSliderFlags_AlwaysClamp);


    static Vector2 hoverPoints[5];
    Vector2 points[2];
    for (float x = 0; x <= spritesheet.columns * spritesheet.tileSize; x += spritesheet.tileSize) {
        points[0] = Vector2{ x , 0 };
        points[1] = Vector2{ x , (float)spritesheet.rows * spritesheet.tileSize };
        DrawLineEx(points[0], points[1], 1.0f, WHITE);
    }

    for (float y = 0; y <= spritesheet.rows * spritesheet.tileSize; y += spritesheet.tileSize) {
        points[0] = Vector2{ 0 , y };
        points[1] = Vector2{ (float) spritesheet.columns * spritesheet.tileSize, y };
        DrawLineEx(points[0], points[1], 1.0f, WHITE);
    }

    if (spritesheet.tileSelected.x >= 0 && spritesheet.tileSelected.y >= 0 && spritesheet.tileSelected.x < spritesheet.columns && spritesheet.tileSelected.y < spritesheet.rows) {
        //DrawLineStrip(hoverPoints, 5, BEIGE);
        DrawRectangleLinesEx(spritesheet.rec, 2.0f, BEIGE);
    }

    EndTextureMode();

    if (ImGui::Button("Reload Spritesheet")) {
        UnloadTexture(spritesheet.texture);
        UnloadRenderTexture(spritesheet.renderTexture);
        spritesheet.texture = LoadTexture("assets/spritesheets/Grass.png");
        spritesheet.renderTexture = LoadRenderTexture(spritesheet.texture.width, spritesheet.texture.height);
    }

    rlImGuiImage(&spritesheet.renderTexture.texture);
    ImVec2 textureMin = ImGui::GetItemRectMin();

    if (ImGui::IsItemHovered()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
            ImVec2 mouse = ImGui::GetIO().MousePos;

            Vector2 position = Vector2(mouse.x - textureMin.x, mouse.y - textureMin.y);
            spritesheet.tileSelected = ToMapCoord(position, spritesheet.tileSize);
            position.x = spritesheet.tileSelected.x * spritesheet.tileSize;
            position.y = spritesheet.tileSelected.y * spritesheet.tileSize;
            spritesheet.rec = Rectangle(position.x, position.y, spritesheet.tileSize, spritesheet.tileSize);
            hoverPoints[0] = { (float)spritesheet.tileSelected.x * spritesheet.tileSize, (float)spritesheet.tileSelected.y * spritesheet.tileSize };
            hoverPoints[1] = { (float)(spritesheet.tileSelected.x + 1) * spritesheet.tileSize, (float)spritesheet.tileSelected.y * spritesheet.tileSize };
            hoverPoints[2] = { (float)(spritesheet.tileSelected.x + 1) * spritesheet.tileSize, (float)(spritesheet.tileSelected.y + 1) * spritesheet.tileSize };
            hoverPoints[3] = { (float)spritesheet.tileSelected.x * spritesheet.tileSize, (float)(spritesheet.tileSelected.y + 1) * spritesheet.tileSize };
            hoverPoints[4] = { (float)spritesheet.tileSelected.x * spritesheet.tileSize, (float)spritesheet.tileSelected.y * spritesheet.tileSize };
            ALK_LOG("Mouse on texture: (%.3f, %.3f)\n", position.x, position.y);
            ALK_LOG("Mouse on texture: (%d, %d)\n", spritesheet.tileSelected.x, spritesheet.tileSelected.y);
        }

    }
    ImGui::End();
}