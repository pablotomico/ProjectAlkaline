#pragma once

#include <iostream>
#include "BaseComponent.h"
#include "include/raylib.h"
#include <vector>
#include <optional>
#include "systems/RenderSystem.h"

namespace alk
{
    class RenderComponent : public BaseComponent
    {
    private:
        RenderSystem::RenderType renderType;
        RenderSystem::VariantRenderData renderData;

    public:
    
        // Constructor for sprite type render component
        RenderComponent(RenderSystem::RenderType renderType, const char* spriteFileName) : renderType(renderType)
        {
            switch(renderType)
            {
                case RenderSystem::RenderType::Sprite:
                    renderData.emplace<RenderSystem::SpriteRenderData>(spriteFileName);
                    ALK_LOG("Added Sprite Render data to component");
                    break;
            }

            ALK_LOG("RenderComponent created successfully");
        }
        ~RenderComponent() override {
            std::cout << "Render Component Destroyed!" << std::endl;
        }

        const RenderSystem::RenderType GetRenderType() const { return renderType; };

        template <typename T>
        std::optional<std::reference_wrapper<const T>> GetRenderData() const {
            if (auto ptr = std::get_if<T>(&renderData)) {
                return std::cref(*ptr);
            }
            return std::nullopt;
        }
    };
}
