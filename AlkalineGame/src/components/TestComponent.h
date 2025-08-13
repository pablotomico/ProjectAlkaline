#pragma once

#include "components/BaseComponent.h"
#include "serialization/SceneSerializer.h"

namespace oth
{
    class TestComponent : alk::BaseComponent
    {
    public:
        std::string path;
        
        TestComponent(){};

        ALK_COMPONENT_SERIALIZER(TestComponent, 
        {
            ALK_SERIALIZE_VALUE(path);
        });

        ALK_COMPONENT_DESERIALIZER(TestComponent, 
        {
            ALK_DESERIALIZE_VALUE(path);
        });
    };
}