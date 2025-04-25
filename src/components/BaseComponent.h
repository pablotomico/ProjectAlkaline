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
        virtual void Update(const float deltaTime) {
            ALK_LOG("Base Component Created");
        };

        void SetOwner(Entity* entity){
            owner = entity;
        }
    };
}