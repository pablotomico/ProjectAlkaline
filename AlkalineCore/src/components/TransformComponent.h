#pragma once

#include "BaseComponent.h"
#include <iostream>
#include "raylib.h"

#include "serialization/SceneSerializer.h"

namespace alk
{
    class TransformComponent : public BaseComponent
    {
    public:
        Vector2 position;
        TransformComponent() : position(Vector2{ 0, 0 }) {}
        TransformComponent(Vector2 position) : position(position) {}

        Vector2 ToIso()
        {
            return Vector2{ position.x - position.y, (position.x + position.y) / 2 }; // TODO: add z --> (x + y)/2 - z
        }

        ALK_COMPONENT_SERIALIZER(TransformComponent,
            {
                ALK_SERIALIZE_VECTOR2(position);
            });

        ALK_COMPONENT_DESERIALIZER(TransformComponent,
            {
                ALK_DESERIALIZE_VECTOR2(position);
            });
    };
}