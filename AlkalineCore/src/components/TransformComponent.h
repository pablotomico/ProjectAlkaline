#pragma once

#include "BaseComponent.h"
#include <iostream>
#include "raylib.h"

namespace alk
{
    class TransformComponent : public BaseComponent 
    {
    public:
        Vector2 position;
        TransformComponent() : position(Vector2{0, 0}) {};
        TransformComponent(Vector2 position) : position(position) {};
    };
}