#pragma once

#include "BaseComponent.h"
#include <iostream>
#include "include/raylib.h"

namespace alk
{
    class TransformComponent : public BaseComponent {

        Vector2 position = { 50, 50 };

    public:
        TransformComponent(){
            ALK_LOG("Transform Component Created!");
        };
        ~TransformComponent(){
        };
        void Update(float deltaTime) override {};
        const Vector2 GetPosition() const { return position; };
    };
}