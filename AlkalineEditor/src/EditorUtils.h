#pragma once

#include "raylib/raylib.h"

inline float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}