#ifdef DEBUG_BUILD

#include "DebugUI.h"
#include "rlImGui.h"
#include "imgui.h"

namespace alk
{
    namespace Debug
    {
        void UI::RegisterButton(const char* id, std::function<void()> callback) {
            buttons.push_back({ id, callback });
        }

        void UI::Draw() {

            rlImGuiBegin();
            ImGui::Begin("Debug");
            ImGui::Text("FPS: %d", GetFPS());

            for (const auto& button : buttons) {
                if (ImGui::Button(button.id)) {
                    button.callback();
                }
            }

            ImGui::End();
            // end ImGui Content
            rlImGuiEnd();
        }
    }
}

#endif // DEBUG_BUILD