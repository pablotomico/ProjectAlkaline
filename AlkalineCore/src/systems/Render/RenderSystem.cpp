#include "systems/Render/RenderSystem.h"
#include "systems/Render/RenderSubsystem.h"

#include "systems/GameLogic/Scene.h"
#include "systems/GameLogic/GameLogic.h"
#include "systems/GameLogic/World.h"

#include "components/RenderComponent.h"
#include "components/TransformComponent.h"
#include "components/GridPreviewComponent.h"
#include "components/GridComponent.h"
#include "components/GridEntityComponent.h"
#include "components/SpriteComponent.h"

#include "misc/GridHelpers.h"

// #include "tracy/Tracy.hpp"

namespace alk
{
    RenderSystem::RenderSystem(CoreSystems& coreSystems) : BaseSystem(coreSystems) {}

    void RenderSystem::Initialize(Scene& scene)
    {

        for (auto& pair : GetFactoryList())
        {
            auto system = pair.second();
            AddSubsystem(pair.first, system);
        }
        // TODO: find CameraComponents
        GetMainCamera() = &mainCamera;
        mainCamera = { 0 };
        mainCamera.target = Vector2{ 0.0f, 0.0f };
        mainCamera.offset = Vector2{ 0.0f, 0.0f };
        mainCamera.rotation = 0.0f;
        mainCamera.zoom = 1.0f;

        World& world = alk::GameLogic::GetWorld();
        auto spriteComponents = world.GetComponents<SpriteComponent>();
        for (auto& component : *spriteComponents)
        {
            component.texHandler = LoadRenderSystemTexture(component.path.c_str());
            component.texture = drawableData.loadedTextures[component.texHandler];
        }

        for (RenderSubsystem* system : GetSubsystems())
        {
            system->SetEnabled(system->Initialize());
        }

        alk::GameLogic::SubscribeToEntitySpawned([this](EntityId id) {
            auto c = alk::GameLogic::GetWorld().GetComponent<SpriteComponent>(id);
            c->texHandler = LoadRenderSystemTexture(c->path.c_str());
            c->texture = drawableData.loadedTextures[c->texHandler];
            });
    }


    void RenderSystem::Reflect(ScriptSystem& script)
    {}

    void RenderSystem::Update(const float deltaTime)
    {}

    void RenderSystem::Shutdown()
    {}

    std::map<std::type_index, RenderSubsystemFactoryFn>& RenderSystem::GetFactoryList()
    {
        static std::map<std::type_index, RenderSubsystemFactoryFn> factories;
        return factories;
    }

    void RenderSystem::AddToRenderPool(alk::EntityId entityId, alk::TransformComponent* transform, alk::SpriteComponent* sprite)
    {
        RenderEntry entry;
        entry.entityId = entityId;
        entry.projection = transform->ToIso();
        entry.texture = sprite->texture;
        entry.color = sprite->color;
        entry.sortKey = transform->position.x + transform->position.y; // TODO: add Z
        
        renderPool.push_back(entry);
    }

    void RenderSystem::AddToScreen(Entity& entity)
    {
        alk::Scene* activeScene = alk::GameLogic::GetActiveScene();
        World& world = activeScene->GetWorld();
        world.GetComponent<RenderComponent>(entity)->SetVisible(true);
        RenderSystemData& renderData = GetRenderSystemData();
        renderData.dirtyLayers = true;

        ALK_LOG("Added entity to screen");
    }

    void RenderSystem::Draw()
    {
        World& world = alk::GameLogic::GetWorld();
        auto spriteComponents = world.GetComponents<SpriteComponent>();
        auto transformComponents = world.GetComponents<TransformComponent>();

        renderPool.clear();
        renderPool.reserve(spriteComponents->Size());

        /// Build render pool
        for (auto i = 0; i < spriteComponents->components.size(); ++i)
        {
            auto [entityId, spriteComponent] = spriteComponents->Get(i);
            if (spriteComponent->visible)
            {
                TransformComponent* transformComponent = transformComponents->Get(entityId);
                AddToRenderPool(entityId, transformComponent, spriteComponent);
            }
        }

        /// Sort
        std::sort(renderPool.begin(), renderPool.end(), [&](RenderEntry a, RenderEntry b) {
                return a.sortKey < b.sortKey;
                });

        /// Draw
        // ZoneScoped;
        BeginMode2D(mainCamera);

        EvaluateAndSortDirtyLayers();

        for(auto& renderEntry : renderPool)
        {
            DrawTexture(renderEntry.texture, (int) renderEntry.projection.x, (int) renderEntry.projection.y, renderEntry.color);   
        }

        EndMode2D();
    }

    void RenderSystem::EvaluateAndSortDirtyLayers()
    {
        RenderSystemData& renderData = GetRenderSystemData();
        if (renderData.dirtyLayers)
        {
            World& world = alk::GameLogic::GetWorld();
            auto renderComponents = world.GetComponents<RenderComponent>();
            auto gridEntityComponents = world.GetComponents<GridEntityComponent>();
            renderData.drawables.clear();

            for (auto i = 0; i < renderComponents->components.size(); ++i)
            {
                RenderComponent& renderComponent = renderComponents->components[i];
                EntityId id = renderComponents->entities[i];

                auto it = gridEntityComponents->entityIndices.find(id);
                if (it != gridEntityComponents->entityIndices.end())
                {
                    size_t gridEntityIndex = it->second;
                    GridEntityComponent& gridEntityComponent = gridEntityComponents->components[gridEntityIndex];
                    renderComponent.SetDrawLayer((uint) gridEntityComponent.GetGridPosition().x + (uint) gridEntityComponent.GetGridPosition().y);
                }

                renderData.drawables.push_back(id);
            }

            std::sort(renderData.drawables.begin(), renderData.drawables.end(), [&](EntityId a, EntityId b) {
                size_t aIndex = renderComponents->entityIndices[a];
                size_t bIndex = renderComponents->entityIndices[b];
                RenderComponent& aRenderComponent = renderComponents->components[aIndex];
                RenderComponent& bRenderComponent = renderComponents->components[bIndex];
                return aRenderComponent.GetDrawLayer() < bRenderComponent.GetDrawLayer();
                });

            ALK_LOG("Sorted render components by draw layer");
            renderData.dirtyLayers = false;
        }
    }

    void RenderSystem::DrawGrid(alk::GridComponent* gridComponent, TransformComponent* transformComponent)
    {
        // ZoneScoped;
        std::vector<Vector2>& gridPoints = gridComponent->gridPoints;

        for (Vector2 point : gridPoints)
        {
            auto endPosX = (int) point.x + gridComponent->tileWidthHalf;
            auto endPosY = (int) point.y + gridComponent->tileHeightHalf;
            DrawLine((int) point.x, (int) point.y, endPosX, endPosY, WHITE);
            DrawLine((int) point.x - gridComponent->tileWidthHalf, (int) point.y + gridComponent->tileHeightHalf, endPosX - gridComponent->tileWidthHalf, endPosY + gridComponent->tileHeightHalf, WHITE);

            DrawLine((int) point.x, (int) point.y, (int) point.x - gridComponent->tileWidthHalf, (int) point.y + gridComponent->tileHeightHalf, WHITE);
            DrawLine(endPosX, endPosY, endPosX - gridComponent->tileWidthHalf, endPosY + gridComponent->tileHeightHalf, WHITE);
        }

        alk::World& world = alk::GameLogic::GetWorld();
        auto gridPreviewComponents = world.GetComponents<alk::GridPreviewComponent>();

        if (gridPreviewComponents->components.size() == 0)
        {
            return;
        }

        RenderSystemData& renderSystemData = GetRenderSystemData();

        for (auto i = 0; i < gridPreviewComponents->components.size(); ++i)
        {
            auto& gridPreviewComponent = gridPreviewComponents->components[i];
            std::vector<std::pair<Vector2, bool>>& validMap = gridPreviewComponent.GetValidMap();

            for (size_t i = 0; i < validMap.size(); i++)
            {
                Vector2 gridPosition = validMap[i].first;
                Vector2 worldPosition = alk::GridHelpers::GridToWorldPosition(gridPosition, gridComponent->width, gridComponent->height, gridComponent->tileWidthHalf, gridComponent->tileHeightHalf, transformComponent->position);
                worldPosition.x -= (gridComponent->tileWidthHalf);
                if (validMap[i].second)
                {
                    auto tex = renderSystemData.loadedTextures[gridComponent->validTileTexHandler];
                    DrawTexture(tex, (int) worldPosition.x, (int) worldPosition.y, Color{ 255, 255, 255, 100 });
                }
                else
                {
                    auto tex = renderSystemData.loadedTextures[gridComponent->invalidTileTexHandler];
                    DrawTexture(tex, (int) worldPosition.x, (int) worldPosition.y, Color{ 255, 255, 255, 100 });
                }
            }
        }
    }

    TextureHandler RenderSystem::LoadRenderSystemTexture(const char* filename)
    {
        RenderSystemData& renderData = GetRenderSystemData();

        auto it = renderData.loadedHandlers.find(filename);
        if (it != renderData.loadedHandlers.end())
        {
            // Texture has already been previously loaded, return saved texture
            return it->second;
        }

        TextureHandler texHandler = GetNextTextureHandler();
        Texture2D texture = LoadTexture(filename);
        renderData.loadedHandlers.insert({ filename, texHandler });
        renderData.loadedTextures.insert({ texHandler, texture });
        return texHandler;
    }

    Camera2D*& RenderSystem::GetMainCamera()
    {
        static Camera2D* mainCamera;
        return mainCamera;
    }
}
