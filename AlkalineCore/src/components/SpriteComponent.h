#pragma once

#include "raylib/raylib.h"
#include "systems/Render/RenderSystemDefinitions.h"
#include "components/BaseComponent.h"
#include "serialization/Serialization.h"

namespace alk
{
    class SpriteComponent : public BaseComponent
    {
    public:
        std::string path;
        alk::TextureHandler texHandler;
        Texture2D texture;
        bool visible = true;
        Color color = WHITE;

        SpriteComponent() {}

        ALK_COMPONENT_SERIALIZER(SpriteComponent,
            {
                ALK_SERIALIZE_VALUE(path);
                ALK_SERIALIZE_VALUE(visible);
            });

        ALK_COMPONENT_DESERIALIZER(SpriteComponent,
            {
                ALK_DESERIALIZE_VALUE(path);
                ALK_DESERIALIZE_VALUE(visible);
            });
    };
}