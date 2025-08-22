#pragma once

#include <string>
#include <map>

namespace alk
{
    namespace InputSystem
    {
        struct InputContext;

        using InputContextMap = std::map<std::string, InputContext>;

        void Initialize();
        void Update(const float deltaTime);
        void Shutdown();

        bool IsKeyDown(int key);
        void LoadInputContext(const std::string& name);
        InputContext*& GetActiveInputContext();
        InputContextMap& GetInputContexts();
    }
}