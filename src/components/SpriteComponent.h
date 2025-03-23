#pragma once

#include <iostream>
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "include/raylib.h"

// Custom deleter for unique_ptr
struct TextureDeleter {
    void operator()(Texture2D* texture) const {
        if (texture) {
            UnloadTexture(*texture);  // Free texture memory
            delete texture;
        }
    }
};

class SpriteComponent : public BaseComponent
{
private:
    std::unique_ptr<Texture2D, TextureDeleter> texture;

public:
    SpriteComponent();
    ~SpriteComponent();

    void Update(const float deltaTime) override;
    void Draw();
    bool LoadSprite(const char* filename);
};