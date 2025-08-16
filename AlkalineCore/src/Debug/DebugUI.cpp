#ifdef ALK_DEBUG

#include "DebugUI.h"
#include "rlImGui.h"
#include "imgui.h"

#include "serialization/SceneSerializer.h"

#include "systems/ScriptSystem.h"
#include "systems/GameLogic.h"
#include "systems/subsystems/GameLogicSubsystem.h"

namespace alk
{
    namespace Debug
    {
        void UI::RegisterText(const char* id, const char* text)
        {
            textMap[id] = text;
        }

        void UI::RegisterButton(const char* id, std::function<void()> callback)
        {
            buttonMap[id] = { callback };
        }

        void UI::UnregisterButton(const char* id)
        {
            auto it = buttonMap.find(id);
            if (it != buttonMap.end())
            {
                buttonMap.erase(it);
            }
        }

        void UI::Draw()
        {

            rlImGuiBegin();

            ImGui::Begin("Debug");
            ImGui::Text("FPS: %d", GetFPS());

            for (const auto& text : textMap)
            {
                ImGui::Text("%s: %s", text.first, text.second);
            }
            ImGui::Separator();
            if (ImGui::Button("Run Lua Test"))
            {
                alk::ScriptSystem::RunFile(std::string(GetWorkingDirectory()) + "/AlkalineGame/scripts/test.lua");
            }

            if (ImGui::Button("Send TestNotification"))
            {
                alk::ScriptSystem::SendNotification("TestNotification", 12, "param");
            }

            ImGui::Separator();
            ImGui::Text("Buttons");
            for (const auto& button : buttonMap)
            {
                if (ImGui::Button(button.first))
                {
                    button.second.callback();
                }
            }

            ImGui::End();

            ImGui::Begin("Game Logic Subsystems");
            for (auto system : alk::GameLogic::GetSubsystems())
            {
                if (system->IsEnabled())
                {
                    ImGui::TextColored(ImVec4(1,1,1,1), "%s", system->GetName().c_str());
                }
                else
                {
                    ImGui::TextColored(ImVec4(1,1,1,0.3f), "%s",system->GetName().c_str());
                }
            }
            ImGui::End(); // Active Systems

            ImGui::Begin("World Outliner");
            World& world = alk::GameLogic::GetWorld();
            for (auto& pair : world.GetAllEntities())
            {
                EntityMeta e = pair.second;
                if (e.valid)
                {
                    ImGui::Text(e.name.c_str());
                }
                else
                {
                    ImGui::TextDisabled(e.name.c_str());
                }
            }

            ImGui::Separator();
            if (ImGui::Button("Test Serialize"))
            {
                sol::table table = alk::ScriptSystem::GetState().create_table();
                alk::GameLogic::Scene* scene = alk::GameLogic::GetActiveScene();
                alk::SceneSerializer::SerializeScene(*alk::GameLogic::GetActiveScene(), table);

                alk::ScriptSystem::SaveTableToFile("C:/dev/ProjectAlkaline/AlkalineGame/scenes/testexport.scene", table);
            }

            ImGui::End(); // World Outliner

            rlImGuiEnd();
        }
    }
}

#endif // ALK_DEBUG

