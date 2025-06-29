#pragma once

#include "sol.hpp"

#include "systems/Scene.h"

namespace alk
{
    namespace SceneLoader
    {
        void DeserializeScene(alk::GameLogic::Scene& scene, sol::table table);
        sol::table SerializeScene(alk::GameLogic::Scene& scene);
    }
}