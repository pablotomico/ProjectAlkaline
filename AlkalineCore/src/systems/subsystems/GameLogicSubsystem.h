#pragma once

#include "alkaline_lib.h"

#include "systems/GameLogic.h"

// GameLogicSubsystem::GameLogicSubsystem -> This is so it can use the base constructor
#define ALK_GAMELOGICSUBSYSTEM(T) \
    public: \
        using GameLogicSubsystem::GameLogicSubsystem; \
        static GameLogicSubsystem* Create() { return new T(std::string(#T)); } \
    private: \
        static inline bool isRegistered = alk::GameLogic::RegisterSystemFactory<T>(Create); \

namespace alk
{
    namespace GameLogic
    {
        class GameLogicSubsystem
        {
        private:
            std::string name;

        public:
            GameLogicSubsystem(std::string name);

            virtual ~GameLogicSubsystem() {}

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