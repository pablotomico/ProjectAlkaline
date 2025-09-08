#pragma once

#include "alkaline_lib.h"
#include "EditorUtils.h"

#include <vector>

class MapChunk
{
public:
    MapChunk(int width, int height) : width(width), height(height)
    {
        Init();
    }

    MapChunk(int size) : width(size), height(size)
    {
        Init();
    }

    int* operator[](size_t i)
    {
        return &(tiles[i * width]);
    }

    size_t Size() const
    {
        return tiles.size();
    }

    int width;
    int height;
private:
    void Init()
    {
        tiles.reserve(width * height);

        for (int i = 0; i < tiles.capacity(); i++) {
            tiles.push_back(-1);
        }
    }

    std::vector<int> tiles;
};

struct SpriteSheet
{
    int tileSize = 64;
    int rows = 1;
    int columns = 4;
    Vector2i tileSelected;
    Texture2D texture;
    RenderTexture2D renderTexture;
    Rectangle rec;

    Rectangle operator[](int i) const
    {
        Vector2i tile = Vector2i(i % columns, i / columns);
        return Rectangle(tile.x * tileSize, tile.y * tileSize, tileSize, tileSize);
    }
};

class MapEditor
{
public:
    MapEditor(bool& show, RenderTexture2D& texture) : show(show), texture(texture), map(10)
    {
        ALK_LOG("[MapEditor] Initilizng camera texture %dx%d", texture.texture.width, texture.texture.height);
        camera.target = Vector2(0, 0);
        camera.offset = Vector2(0, 0);
        // camera.offset = Vector2(texture.texture.width * 0.5f, 0);
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        tileSizeHalf = tileSize / 2;

        spritesheet.tileSelected = Vector2i(0, 0);
        spritesheet.texture = LoadTexture("assets/spritesheets/Grass.png");
        spritesheet.renderTexture = LoadRenderTexture(spritesheet.texture.width, spritesheet.texture.height);

        ALK_LOG("[MapEditor] test map operator %d", map[1][2]);
    }

    void Update(const float deltaTime);
    void Draw();

private:
    void DrawGrid();
    void DrawMapChunk(MapChunk& map) const;
    
    void DrawToolbar();

    bool& show;
    RenderTexture2D& texture;
    Camera2D camera;
    MapChunk map;
    SpriteSheet spritesheet;

    int tileSize = 32;
    int tileSizeHalf;
};