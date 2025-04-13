#pragma once

#include "BaseComponent.h"
#include <iostream>
#include "include/raylib.h"

namespace alk
{
    class TransformComponent : public BaseComponent {

        std::vector<Vector2> positionArray;

    public:
        TransformComponent(){
            ALK_LOG("Transform Component Created!");
        };
        // Constructor for single-transform component
        TransformComponent(Vector2 position){
            positionArray.clear();
            positionArray.push_back(position);
            ALK_LOG("Transform Component Created!");
        };
        // Constructor which reserves n-number of elements worth of space in position array
        TransformComponent(uint16_t elementCount){
            positionArray.reserve(elementCount);
            ALK_LOG("Transform Component Created!");
        };
        ~TransformComponent(){
        };
        void Update(float deltaTime) override {};
        const Vector2 GetPosition() const { return positionArray[0]; };

        std::vector<Vector2>& GetPositionArray() { return positionArray; };
        const std::vector<Vector2>& GetPositionArray() const { return positionArray; };
    };
}