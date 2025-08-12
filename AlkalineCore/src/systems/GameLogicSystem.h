#pragma once

#include "alkaline_lib.h"

#include "systems/GameLogic.h"

// GameLogicSystem::GameLogicSystem -> This is so it can use the base constructor
#define ALK_GAMELOGICSYSTEM(T) \
    public: \
        using GameLogicSystem::GameLogicSystem; \
        static GameLogicSystem* Create() { return new T(std::string(#T)); } \
    private: \
        static inline bool isRegistered = alk::GameLogic::RegisterSystemFactory<T>(Create); \

namespace alk
{
    namespace GameLogic
    {
        class GameLogicSystem
        {
        private:
            std::string name;

        public:
            GameLogicSystem(std::string name);

            virtual ~GameLogicSystem() {}

            // IDEA: pass current world to these
            virtual bool Initialize() { return false; };
            virtual void Update() {};
            virtual void Shutdown() {};

            std::string GetName()
            {
                return name;
            }
        };
    }
}