#pragma once

#include <string>

namespace alk
{
    class Scene;
    class ScriptSystem;
    
    class BaseSystem
    {
    public:
        BaseSystem() {}
        virtual void Initialize(Scene& scene) {};
        virtual void Reflect(ScriptSystem& script) {};
        virtual void Update(const float deltaTime) {};
        virtual void Shutdown() {};
    };
}