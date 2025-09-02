#pragma once

#include "systems/GameLogic/GameLogic.h"
#include "systems/GameLogic/GameLogicSubsystem.h"

namespace oth
{
    class TestSubsystem : public alk::GameLogicSubsystem
    {
        ALK_GAMELOGICSUBSYSTEM(TestSubsystem);

    public:
        bool Initialize() override;
        void Update() override {};
        void Shutdown() override {};
    };
}