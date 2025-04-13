#pragma once

namespace alk
{
    class BaseEntity;
    
    class BaseComponent
    {
    protected:
        BaseEntity* owner;
    public:
        virtual ~BaseComponent() = default;
        virtual void Update(const float deltaTime) {
            ALK_LOG("Base Component Created");
        };

        void SetOwner(BaseEntity* entity){
            owner = entity;
        }
    };
}