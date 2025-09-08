#pragma once

#include "raylib/raylib.h"

struct Vector2i
{
    int x;
    int y;
};

inline float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}

inline Vector2 ToIso(Vector2 point)
{
    return { point.x - point.y, (point.x + point.y) / 2 };
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

inline Vector2 ToCart(const Vector2& point)
{
    return { (point.x * 0.5f) + point.y, point.y - (point.x * 0.5f) };
}

inline Vector2i ToMapCoord(const Vector2& point, int tileSize)
{
    return { (int)(point.x / tileSize), (int)(point.y / tileSize) };
}