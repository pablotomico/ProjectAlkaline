#pragma once

#include "sol.hpp"

#include "systems/Scene.h"

namespace alk
{
    namespace SceneSerializer
    {
        void DeserializeScene(alk::GameLogic::Scene& scene, const sol::table& table);
        sol::table SerializeScene(alk::GameLogic::Scene& scene);
    }
}