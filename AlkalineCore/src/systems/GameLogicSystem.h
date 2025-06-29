#pragma once
#include "alkaline_lib.h"

namespace alk
{
    namespace GameLogic
    {
        class GameLogicSystem
        {
        private:
            const char *name;

        public:
            GameLogicSystem(const char *name);

            virtual ~GameLogicSystem() {}

            virtual void Initialize() {};
            virtual void Update() {};
            virtual void Shutdown() {};

            const char *GetName()
            {
                return name;
            }
        };
    }
}