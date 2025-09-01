#pragma once

#include "BaseComponent.h"
#include "systems/Render/RenderSystemDefinitions.h"
#include "misc/GridHelpers.h"
#include "serialization/SceneSerializer.h"
#include <vector>

namespace alk
{
    class GridComponent : public BaseComponent
    {
    public:
        int width;
        int height;
        bool isStatic = true;
        uint tileWidth;
        uint tileHeight;
        std::string validTileSpritePath;
        std::string invalidTileSpritePath;

        std::vector<Vector2> gridPoints;
        alk::TextureHandler validTileTexHandler;
        alk::TextureHandler invalidTileTexHandler;
        uint tileWidthHalf;
        uint tileHeightHalf;

        void GeneratePoints(Vector2 gridWorldPosition)
        {
            gridPoints.clear();
            gridPoints.reserve(width * height);
            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    if (isStatic)
                    {
                        Vector2 pointWorldPosition = GridHelpers::GridToWorldPosition({ float(i), float(j) }, width, height, tileWidthHalf, tileHeightHalf, gridWorldPosition);
                        gridPoints.emplace_back(pointWorldPosition);
                    }
                    else
                    {
                        // TODO: handle non static grids when (if) needed
                    }
                }
            }
        }

        ALK_COMPONENT_SERIALIZER(GridComponent,
            {
                ALK_SERIALIZE_VALUE(width);
                ALK_SERIALIZE_VALUE(height);
                ALK_SERIALIZE_VALUE(tileWidth);
                ALK_SERIALIZE_VALUE(tileHeight);
                ALK_SERIALIZE_VALUE(validTileSpritePath);
                ALK_SERIALIZE_VALUE(invalidTileSpritePath);
            });

        ALK_COMPONENT_DESERIALIZER(GridComponent,
            {
                ALK_DESERIALIZE_VALUE(width);
                ALK_DESERIALIZE_VALUE(height);
                ALK_DESERIALIZE_VALUE(tileWidth);
                ALK_DESERIALIZE_VALUE(tileHeight);
                ALK_DESERIALIZE_VALUE(validTileSpritePath);
                ALK_DESERIALIZE_VALUE(invalidTileSpritePath);

                component->tileWidthHalf = component->tileWidth / 2;
                component->tileHeightHalf = component->tileHeight / 2;
            });
    };
}