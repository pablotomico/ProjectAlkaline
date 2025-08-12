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
        TransformComponent() : position(Vector2{0, 0}) {};
        TransformComponent(Vector2 position) : position(position) {};

        ALK_COMPONENT_SERIALIZER(TransformComponent, 
        {
            table["position"] = lua.create_table();
            table["position"]["x"] = component->position.x;
            table["position"]["y"] = component->position.y;
        });

        ALK_COMPONENT_DESERIALIZER(TransformComponent,
        {
            component->position.x = table["position"]["x"];
            component->position.y = table["position"]["y"];
        });
    };

}