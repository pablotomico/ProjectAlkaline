#pragma once

namespace alk
{
    class Entity;
    
    class BaseComponent
    {
    protected:
        Entity* owner;
    public:
        virtual ~BaseComponent() = default;
        void SetOwner(Entity* entity){
            owner = entity;
        }
    };
}