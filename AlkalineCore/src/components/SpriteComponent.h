#pragma once

#include "raylib/raylib.h"
#include "systems/RenderSystem.h"
#include "components/BaseComponent.h"
#include "serialization/Serialization.h"

namespace alk
{
    class SpriteComponent : public BaseComponent
    {
    public:
        std::string path;
        TextureHandler texHandler;
        bool visible = true;
        Color color = WHITE;
        int drawLayer = 0;

        SpriteComponent() {}

        ALK_COMPONENT_SERIALIZER(SpriteComponent,
            {
                ALK_SERIALIZE_VALUE(path);
                // ALK_SERIALIZE_VALUE(drawLayer);
            });

        ALK_COMPONENT_DESERIALIZER(SpriteComponent,
            {
                ALK_DESERIALIZE_VALUE(path);
                // ALK_DESERIALIZE_VALUE(drawLayer);
            });
    };
}