#pragma once

#include "BaseComponent.h"
#include <iostream>
#include "include/raylib.h"

namespace alk
{
    class GridPreviewComponent : public BaseComponent {
        Vector2 dimensions = {0, 0};
    public:
        GridPreviewComponent(Vector2 dimensions) : BaseComponent() {
            this->dimensions = dimensions;
            ALK_LOG("Grid Preview Component Created!");
        };
        ~GridPreviewComponent(){
        };
        void Update(float deltaTime) override {};
    };
}