#include "SpriteComponent.h"
#include "entities/BaseEntity.h"

SpriteComponent::SpriteComponent()
{
    std::cout << "Sprite Component Created!" << std::endl;
}
SpriteComponent::~SpriteComponent(){
    std::cout << "Sprite Component Destroyed!" << std::endl;
}
void SpriteComponent::Update(const float deltaTime)
{
}

/**
 * @brief Draw the sprite texture.
 * Requires the transform component to draw at a position
 * @todo Ensure the texture is valid before drawing
 */
void SpriteComponent::Draw()
{
    //TODO - ensure texture is valid before trying to draw

    if(!owner->HasComponent<TransformComponent>())
    {
        std::cout << "Sprite comopnent requires Transform component to draw sprite" << std::endl;
        return;
    }

    if(auto position = owner->GetComponent<TransformComponent>()->GetPosition().lock())
    {
        DrawTexture(*texture, position->x, position->y, WHITE);  // Dereference to use
    }    
}

/**
 * @brief Load a sprite texture from a file path
 * @todo Ensure the texture is valid before setting
 * @return bool (Whether the file has been successfully loaded)
 */
bool SpriteComponent::LoadSprite(const char* filename)
{
    if(filename == nullptr)
    {
        std::cout << "LoadSprite: received filename is a nullptr" << std::endl;
        return false;
    }
    Texture2D* tempTexture = new Texture2D(LoadTexture(filename));

    //TODO - ensure texture is valid before setting
    texture.reset(tempTexture);
    return true;
}