#pragma once

#include "raylib/raylib.h"

inline float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}

inline void ToIso(Vector2* points, int pointCount)
{
    for (int i = 0; i < pointCount; i++)
    {
        Vector2 cart = points[i];
        points[i].x = cart.x - cart.y;
        points[i].y = (cart.x + cart.y) / 2;
    }
    
}