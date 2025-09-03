#pragma once

#include "alkaline_lib.h"
#include "raylib/raylib.h"

#include <vector>

class Map
{
public:
    Map(int width, int height) : width(width), height(height)
    {
        tiles.reserve(width + height);

        for (int i = 0; i < tiles.capacity(); i++)
        {
            tiles.push_back(i);
        }
        
    }

    int* operator[](size_t i)
    {
        return &(tiles[i * width]);
    }

private:
    int width;
    int height;

    std::vector<int> tiles;
};

class MapEditor
{
public:
    MapEditor(bool& show, RenderTexture2D& texture) : show(show), texture(texture), map(10, 10)
    {
        ALK_LOG("[MapEditor] Initilizng camera texture %dx%d", texture.texture.width, texture.texture.height);
        camera.target = Vector2(0, 0);
        camera.offset = Vector2(texture.texture.width * 0.5f, 0);
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        tileSizeHalf = tileSize / 2;

        ALK_LOG("[MapEditor] test map operator %d", map[1][2]);
    }

    void Draw();

private:
    void DrawGrid();
    void DrawToolbar();

    bool& show;
    RenderTexture2D& texture;
    Camera2D camera;
    Map map;

    int tileSize = 128;
    int tileSizeHalf;
};