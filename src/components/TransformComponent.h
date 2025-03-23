#pragma once

#include "BaseComponent.h"
#include <iostream>
#include "include/raylib.h"

class TransformComponent : public BaseComponent {

    std::shared_ptr<Vector2> position = std::make_unique<Vector2>(50, 50);

public:
    TransformComponent(){
        std::cout << "Transform Component Created!" << std::endl;
    };
    ~TransformComponent(){
        std::cout << "Transform Component Destroyed!" << std::endl;
    };
    void Update(float deltaTime) override {};
    std::weak_ptr<Vector2> GetPosition() const { return position; };
};