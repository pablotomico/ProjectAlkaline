#pragma once

#include "imgui.h"
#include "rlImGui.h"
#include "include/raylib.h"
#include "entities/BaseEntity.h"
#include "components/SpriteComponent.h"
#include "components/TransformComponent.h"

constexpr bool includeDebug = true;

class AlkalineApplication
{
private:
    BaseEntity* entity = new BaseEntity();

public:
    AlkalineApplication();
    ~AlkalineApplication();

    bool Initialize();
    void Update(const float deltaTime);
    void FixedUpdate(const float deltaTime);
    void Draw();
    void Close();
    bool ShouldClose();
};