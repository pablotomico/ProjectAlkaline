#pragma once

#include "alkaline_lib.h"

#include "systems/GameLogic/GameLogic.h"

// RenderSubsystem::RenderSubsystem -> This is so it can use the base constructor
#define ALK_RENDERSUBSYSTEM(T) \
    public: \
        using RenderSubsystem::RenderSubsystem; \
        static RenderSubsystem* Create() { return new T(std::string(#T)); } \
    private: \
        static inline bool isRegistered = alk::RenderSystem::RegisterSystemFactory<T>(Create); \

namespace alk
{
    class RenderSubsystem
    {
    private:
        std::string name;
        bool enabled = false;

    public:
        RenderSubsystem(std::string name);

        virtual ~RenderSubsystem() {}

        virtual bool Initialize() { return false; };
        virtual void Draw() {};
        virtual void Shutdown() {};

        std::string GetName()
        {
            return name;
        }

        bool IsEnabled()
        {
            return enabled;
        }

        void SetEnabled(bool newEnabled)
        {
            enabled = newEnabled;
        }
    };
}