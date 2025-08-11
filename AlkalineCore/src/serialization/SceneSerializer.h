#pragma once

#include "sol.hpp"

#include "systems/Scene.h"

namespace alk
{
    namespace SceneSerializer
    {
        void DeserializeScene(alk::GameLogic::Scene& scene, const sol::table& table);
        void SerializeScene(alk::GameLogic::Scene& scene, sol::table& table);
    }
}