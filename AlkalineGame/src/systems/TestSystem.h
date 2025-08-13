#pragma once

#include "systems/GameLogic.h"
#include "systems/GameLogicSystem.h"

namespace oth
{
    namespace GameLogic
    {
        class TestSystem : public alk::GameLogic::GameLogicSystem
        {
            ALK_GAMELOGICSYSTEM(TestSystem);

        public:
            bool Initialize() override;
            void Update() override {};
            void Shutdown() override {};
        };
    }
}