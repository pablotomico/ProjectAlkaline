#include "systems/Render/RenderSystem.h"
#include "systems/Render/RenderSubsystem.h"

#include "systems/GameLogic/Scene.h"
#include "systems/GameLogic/GameLogic.h"
#include "systems/GameLogic/World.h"

#include "components/TransformComponent.h"
#include "components/SpriteComponent.h"

#include "misc/GridHelpers.h"

// #include "tracy/Tracy.hpp"

namespace alk
{
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
            component.texHandle = LoadRenderSystemTexture(component.path);
        }

        for (RenderSubsystem* system : GetSubsystems())
        {
            system->SetEnabled(system->Initialize());
        }

        alk::GameLogic::SubscribeToEntitySpawned([this](EntityId id) {
            auto c = alk::GameLogic::GetWorld().GetComponent<SpriteComponent>(id);
            c->texHandle = LoadRenderSystemTexture(c->path);
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
        entry.texHandle = sprite->texHandle;
        entry.color = sprite->color;
        entry.sortKey = RenderSystem::CalculateSortKey(transform->position);

        renderPool.push_back(entry);
    }

    void RenderSystem::Draw()
    {
        RenderSystemData& renderData = GetRenderSystemData();
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

        for (auto& renderEntry : renderPool)
        {
            DrawTexture(renderData.loadedTextures[renderEntry.texHandle], (int) renderEntry.projection.x, (int) renderEntry.projection.y, renderEntry.color);
        }

        EndMode2D();
    }



    TextureHandle RenderSystem::LoadRenderSystemTexture(std::string filename)
    {
        RenderSystemData& renderData = GetRenderSystemData();

        if (renderData.textureHandles.contains(filename))
        {
            // Texture has already been previously loaded, return saved texture
            return renderData.textureHandles[filename];
        }

        size_t index = renderData.loadedTextures.size();
        renderData.textureHandles.emplace(filename, index);
        renderData.loadedTextures.push_back(LoadTexture(filename.c_str()));
        return index;
    }

    Camera2D*& RenderSystem::GetMainCamera()
    {
        static Camera2D* mainCamera;
        return mainCamera;
    }

    float RenderSystem::CalculateSortKey(const Vector2 position)
    {
        return position.x + position.y; // TODO: add Z
    }
}
