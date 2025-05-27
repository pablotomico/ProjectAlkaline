#pragma once
#include <functional>

namespace alk
{
    namespace Debug
    {
        namespace UI
        {
            #ifdef DEBUG_BUILD

            struct ButtonData
            {
                const char* id;
                std::function<void()> callback;
            };
            static std::vector<ButtonData> buttons;
            void RegisterButton(const char* id, std::function<void()> callback);
            // TODO: Add function to remove buttons
            void Draw();
    
            #else
    
            inline void RegisterButton(const char*, std::function<void()>){}
            inline void Draw(){}
    
            #endif // DEBUG_BUILD
        }
    }
}