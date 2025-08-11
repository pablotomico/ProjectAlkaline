#ifdef ALK_DEBUG

#include "DebugUI.h"
#include "rlImGui.h"
#include "imgui.h"

#include "systems/GameLogic.h"

namespace alk
{
    namespace Debug
    {
        void UI::RegisterText(const char *id, const char *text)
        {
            textMap[id] = text;
        }

        void UI::RegisterButton(const char* id, std::function<void()> callback) {
            buttonMap[id] = { callback };
        }

        void UI::UnregisterButton(const char *id)
        {
            auto it = buttonMap.find(id);
            if (it != buttonMap.end()) {
                buttonMap.erase(it);
            }
        }

        void UI::Draw() {

            rlImGuiBegin();

            ImGui::Begin("Debug");
            ImGui::Text("FPS: %d", GetFPS());
            
            for (const auto& text : textMap) {
                ImGui::Text("%s: %s", text.first, text.second);
            }
            
            ImGui::Separator();
            ImGui::Text("Buttons");
            for (const auto& button : buttonMap) {
                if (ImGui::Button(button.first)) {
                    button.second.callback();
                }
            }
            
            ImGui::End();

            ImGui::Begin("World Outliner");
            World& world = alk::GameLogic::GetWorld();
            for(auto& pair : world.DebugGetAllEntities())
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
            

            ImGui::End(); // World Outliner

            rlImGuiEnd();
        }
    }
}

#endif // ALK_DEBUG

