#pragma once

#include "systems/BaseSystem.h"
#include "systems/Input/InputContext.h"
#include <string>
#include <map>

namespace alk
{
    struct InputContext;
    using InputContextMap = std::map<std::string, InputContext>;

    class InputSystem : public BaseSystem
    {
    public:
        InputSystem(CoreSystems& coreSystems);
        void Initialize(Scene& scene) override;
        void Reflect(ScriptSystem& script) override;
        void Update(const float deltaTime) override;
        void Shutdown() override;

        bool IsKeyDown(int key);
        void LoadInputContext(const std::string& name);
        InputContext*& GetActiveInputContext();
        InputContextMap& GetInputContexts();

    private:
        alk::InputContext* currentContext = nullptr;
        alk::InputContextMap inputContexts;
    };
}