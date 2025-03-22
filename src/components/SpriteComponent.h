#pragma once

#include "BaseComponent.h"
#include "raylib.h"
#include <iostream>

class SpriteComponent : public BaseComponent
{
public:
    SpriteComponent(){
        std::cout << "Sprite Component Created!" << std::endl;
    };
    ~SpriteComponent(){
        std::cout << "Sprite Component Destroyed!" << std::endl;
    };
    void Update(float deltaTime) override {
        //std::cout << "Sprite Component Updated!" << std::endl;
    };
};