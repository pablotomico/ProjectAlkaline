#pragma once
#include <functional>
#include <map>

namespace alk
{
    namespace Debug
    {
        namespace UI
        {
            #ifdef DEBUG_BUILD

            struct ButtonData
            {
                std::function<void()> callback;
            };
            //static std::vector<ButtonData> buttons;
            static std::unordered_map<const char*, const char*> textMap;
            static std::unordered_map<const char*, ButtonData> buttonMap;

            void RegisterText(const char* id, const char* text);
            void RegisterButton(const char* id, std::function<void()> callback);
            void UnregisterButton(const char* id);

            // TODO: Add function to remove buttons
            void Draw();
    
            #else
    
            inline void RegisterText(const char* id, const char* text){}
            inline void RegisterButton(const char* id, std::function<void()> callback){}
            inline void UnregisterButton(const char* id){}
            inline void Draw(){}
    
            #endif // DEBUG_BUILD
        }
    }
}