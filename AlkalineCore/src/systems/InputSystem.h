#pragma once

namespace alk
{
    namespace InputSystem
    {
        void Initialize();
        void Update(const float deltaTime);
        void Shutdown();

        bool IsKeyDown(int key);
    }
}