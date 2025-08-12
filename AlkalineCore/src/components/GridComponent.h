#pragma once

#include <vector>
#include "raylib.h"
#include "BaseComponent.h"

#include "misc/GridHelpers.h"

namespace alk
{
    namespace GameLogic
    {
        class GridComponent : public BaseComponent 
        {
        public:
            int width;
            int height;
            std::vector<Vector2> gridPoints;
            bool isStatic = true;
            
            GridComponent(int width, int height) : width(width), height(height) {};

            void GeneratePoints(Vector2 worldPosition)
            {
                gridPoints.clear();
                gridPoints.reserve(width * height);
                for (int i = 0; i < width; ++i)
                {
                    for (int j = 0; j < height; ++j)
                    {
                        if (isStatic)
                        {
                            Vector2 pointWorldPosition = GridHelpers::GridToWorldPosition({ float(i), float(j) }, width, height);
                            pointWorldPosition.x += worldPosition.x;
                            pointWorldPosition.y += worldPosition.y;
                            gridPoints.emplace_back(pointWorldPosition);
                        }
                        else
                        {
                            // TODO: handle non static grids when (if) needed
                        }
                    }
                }
            }
        };
    }
}