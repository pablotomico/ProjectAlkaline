#pragma once

#include "systems/GameLogic.h"
#include "systems/GameLogicSubsystem.h"

namespace oth
{
    namespace GameLogic
    {
        class TestSubsystem : public alk::GameLogic::GameLogicSubsystem
        {
            ALK_GAMELOGICSUBSYSTEM(TestSubsystem);

        public:
            bool Initialize() override;
            void Update() override {};
            void Shutdown() override {};
        };
    }
}