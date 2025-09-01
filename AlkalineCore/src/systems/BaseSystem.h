#pragma once

#include "systems/CoreSystems.h"

#include <string>

namespace alk
{
    class Scene;
    class ScriptSystem;
    
    class BaseSystem
    {
    public:
        explicit BaseSystem(CoreSystems& cs) : coreSystems(cs) {}
        virtual void Initialize(Scene& scene) {};
        virtual void Reflect(ScriptSystem& script) {};
        virtual void Update(const float deltaTime) {};
        virtual void Shutdown() {};

    protected:
        CoreSystems& coreSystems;
    };
}