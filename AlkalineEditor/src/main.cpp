#include "alkaline_lib.h"
#include "Application.h"
#include "MapEditor.h"

#include "EditorUtils.h"
#include "rlImGui/rlImGui.h"
#include "imgui/imgui.h"

void DrawTool(RenderTexture2D& renderTexture);

int main()
{
    ALK_TRACE("THIS WILL BE THE ALKALINE EDITOR");
    
    IMGUI_CHECKVERSION();

    SetTraceLogLevel(LOG_NONE);
    InitWindow(1920, 1080, "Alkaline Editor v0.1");
    SetTargetFPS(144);
    rlImGuiSetup(true);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    alk::Application* application = nullptr;

    bool showMapEditor = true;
    RenderTexture2D mapEditorTexture = LoadRenderTexture(1280, 720); // TODO: support resize
    MapEditor mapEditor = MapEditor(showMapEditor, mapEditorTexture);

    
    RenderTexture2D renderTexture;
    RenderTexture2D toolTexture = LoadRenderTexture(1280, 720);
    bool focused = false;
    bool playing = false;
    bool showGame = false;
    bool ImGuiDemoOpen = false;
    bool testTool = false;

    double const fixedTimeStep = 1 / fixedUpdateFPS;
    double nextFixedUpdate = 0;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (application)
        {
            application->Update(deltaTime);

            double currentTime = GetTime();
            if (currentTime > nextFixedUpdate)
            {
                application->FixedUpdate(fixedTimeStep);
                nextFixedUpdate = currentTime + fixedTimeStep;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();
        if (showGame && application)
        {
            application->Draw(&renderTexture);
        }




        // ImGui

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Demo", NULL, &ImGuiDemoOpen, true);
                ImGui::MenuItem("Game", NULL, &showGame, true);
                ImGui::MenuItem("Tool", NULL, &testTool, true);
                ImGui::MenuItem("MapEditor", NULL, &showMapEditor, true);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (ImGuiDemoOpen)
            ImGui::ShowDemoWindow(&ImGuiDemoOpen);

        if (showGame)
        {

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::SetNextWindowSizeConstraints(ImVec2(ScaleToDPIF(1280.0f), ScaleToDPIF(720.0f)), ImVec2((float) GetScreenWidth(), (float) GetScreenHeight()));
            ImGui::Begin("Game", &showGame);
            if (application)
            {
                if (ImGui::Button("Stop"))
                {
                    UnloadRenderTexture(renderTexture);
                    application->Shutdown();
                    delete(application);
                    application = nullptr;
                }
                else
                {
                    rlImGuiImageRenderTextureFit(&renderTexture, true);
                }
            }
            else
            {
                if (ImGui::Button("Play"))
                {
                    renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
                    application = new alk::Application();
                    ALK_ASSERT(application->Initialize("C:/dev/ProjectAlkaline/AlkalineGame/scenes/test.scene", false), "Critical failure during initialization, closing application");
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }

        if (testTool)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Tool", &testTool, ImGuiWindowFlags_AlwaysAutoResize);
            DrawTool(toolTexture);
            rlImGuiImage(&toolTexture.texture);
            ImGui::End();
            ImGui::PopStyleVar();
        }

        
        if(showMapEditor)
        {
            mapEditor.Update(deltaTime);
            mapEditor.Draw();
        }

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}

void CartesianToIsometric(Vector2& v, const int tileWidthHalf, const int tileHeightHalf)
{
    float cartX = v.x;
    float cartY = v.y;
    v.x = (cartX - cartY) * tileWidthHalf;
    v.y = (cartX + cartY) * tileHeightHalf;
}

struct Bounds3D
{
    Vector3 bounds[2];

    Bounds3D(Vector3 min, Vector3 max)
    {
        bounds[0] = min;
        bounds[1] = max;
    }
    void ToPoints(Vector3 points[8])
    {
        for (int i = 0; i < 8; i++)
        {
            points[i].x = bounds[(i ^ (i >> 1)) & 1].x;
            points[i].y = bounds[(i >> 1) & 1].y;
            points[i].z = bounds[(i >> 2) & 1].z;
        }
    }
};

void DrawTool(RenderTexture2D& renderTexture)
{
    BeginTextureMode(renderTexture);
    ClearBackground(DARKGRAY);

    enum TileWidth { _32, _64, _128, _256, COUNT };
    static int elem = _128;
    const char* elems_names[COUNT] = { "32", "64", "128", "256" };
    const int elems_values[COUNT] = { 32, 64, 128, 256 };
    const char* elem_name = (elem >= 0 && elem < COUNT) ? elems_names[elem] : "???";
    static int tileWidth = elems_values[elem];
    static int tileHeight = tileWidth / 2;
    static int tileWidthHalf = tileHeight;
    static int tileHeightHalf = tileHeight / 2;
    if (ImGui::SliderInt("Tile Size", &elem, 0, COUNT - 1, elem_name, ImGuiSliderFlags_NoInput))
    {
        tileWidth = elems_values[elem];
        tileHeight = elems_values[elem] / 2;
        tileWidthHalf = tileHeight;
        tileHeightHalf = tileHeight / 2;
        ALK_LOG("Tile Size is now %ix%i", tileWidth, tileHeight);
    }
    static float boundSizes[3] = { 1, 1, 1 };
    // ImGui::InputFloat3("Bounds", boundSizes);
    ImGui::SliderFloat3("Bounds", boundSizes, 0.33f, 10, "%.2f", ImGuiSliderFlags_AlwaysClamp);
    static float position[2] = { 0, 0 };
    ImGui::SliderFloat2("Position", position, -300, 300);

    // Cartesian Points
    Vector2 points[10] =
    {
        {0, 0}, { boundSizes[0], 0 }, {boundSizes[0], boundSizes[1]}, {0, boundSizes[1]}, {0, 0},
        {boundSizes[2], boundSizes[2]}, { boundSizes[0] + boundSizes[2], boundSizes[2] }, {boundSizes[0] + boundSizes[2], boundSizes[1] + boundSizes[2]}, {boundSizes[2], boundSizes[1] + boundSizes[2]}, {boundSizes[2], boundSizes[2]}
    };


    for (int i = 0; i < 10; i++)
    {
        CartesianToIsometric(points[i], tileWidthHalf, tileHeightHalf);
        points[i].x += 640 + position[0];
        points[i].y += 256 + position[1];
    }

    DrawLineStrip(points, 10, SKYBLUE);
    DrawLineV(points[1], points[6], SKYBLUE);
    DrawLineV(points[2], points[7], SKYBLUE);
    DrawLineV(points[3], points[8], SKYBLUE);



    EndTextureMode();
}
