#pragma once

#include <iostream>

class BaseEntity;

class BaseComponent
{
protected:
    BaseEntity* owner;
public:
    virtual ~BaseComponent() = default;
    virtual void Update(const float deltaTime) {
        std::cout << "Base Component Updated!" << std::endl;
    };

    void SetOwner(BaseEntity* entity){
        owner = entity;
    }
};