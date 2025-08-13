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
        TransformComponent() : position(Vector2{0, 0}) {}
        TransformComponent(Vector2 position) : position(position) {}

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