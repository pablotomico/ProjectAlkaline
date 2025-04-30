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
        bool visible = false;
        Color color = WHITE;
        Vector2 dimensions;
    public:
    
        RenderComponent()
        {
            ALK_LOG("RenderComponent created successfully");
        }
        // Constructor for sprite type render component
        RenderComponent(RenderSystem::RenderType renderType, const char* spriteFileName, Color colorOverride = WHITE) 
            : renderType(renderType),
            color(colorOverride)
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

        // Constructor for grid type render component
        RenderComponent(RenderSystem::RenderType renderType, uint tileWidthHalf, uint tileHeightHalf) : renderType(renderType)
        {
            switch(renderType)
            {
                case RenderSystem::RenderType::Grid:
                    renderData.emplace<RenderSystem::GridRenderData>(tileWidthHalf, tileHeightHalf);
                    ALK_LOG("Added Grid Render data to component");
                    break;
            }

            ALK_LOG("RenderComponent created successfully");
        }

        ~RenderComponent() override {
            std::cout << "Render Component Destroyed!" << std::endl;
        }

        const RenderSystem::RenderType GetRenderType() const { return renderType; };

        template <typename T>
        const std::optional<std::reference_wrapper<const T>> GetRenderData() const {
            if (auto ptr = std::get_if<T>(&renderData)) {
                return std::cref(*ptr);
            }
            return std::nullopt;
        }

        void SetVisible(bool newVisibility)
        {
            visible = newVisibility;
        }

        inline const bool GetVisible() const
        {
            return visible;
        }

        inline const Color GetColor() const
        {
            return color;
        }
    };
}
