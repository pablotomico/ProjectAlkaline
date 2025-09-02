#pragma once

#include "alkaline_lib.h"
#include "systems/BaseSystem.h"
#include "systems/Render/RenderSystemDefinitions.h"
#include <unordered_map>
#include <variant>
#include <map>
#include <typeindex>
#include <functional>

#include "entities/Entity.h"

namespace alk
{
    class Scene;
    class World;
    class RenderSubsystem;
    class TransformComponent;
    class SpriteComponent;

    using RenderSubsystemFactoryFn = RenderSubsystem * (*)();

    class RenderSystem : public BaseSystem
    {
    private:
        std::vector<RenderEntry> renderPool;
        RenderSystemData drawableData;
        Camera2D mainCamera;

    public:
        RenderSystem(CoreSystems& coreSystems);
        void Initialize(Scene& scene) override;
        void Reflect(ScriptSystem& script) override;
        void Update(const float deltaTime) override;
        void Shutdown() override;

        void Draw();


        static std::map<std::type_index, RenderSubsystemFactoryFn>& GetFactoryList();

        template <typename T>
        inline static bool RegisterSystemFactory(RenderSubsystemFactoryFn factory)
        {
            GetFactoryList().emplace(typeid(T), factory);
            return true;
        }


        inline std::vector<RenderSubsystem*>& GetSubsystems()
        {
            static std::vector<RenderSubsystem*> subsystems;
            return subsystems;
        }

        inline std::unordered_map<std::type_index, RenderSubsystem*>& GetSubsystemsMap()
        {
            static std::unordered_map<std::type_index, RenderSubsystem*> subsystemsMap;
            return subsystemsMap;
        }

        template <typename T>
        inline T* GetSubsystem()
        {
            return static_cast<T*>(GetSubsystemsMap()[typeid(T)]);
        }

        inline std::vector<std::function<void(EntityId)>>& GetCallbacks()
        {
            static std::vector<std::function<void(EntityId)>> callbacks;
            return callbacks;
        }

        inline void AddSubsystem(std::type_index type, RenderSubsystem* system)
        {
            GetSubsystems().push_back(system);
            GetSubsystemsMap().emplace(type, system);
        }

        struct TextureDeleter
        {
            void operator()(Texture2D* texture) const
            {
                if (texture)
                {
                    UnloadTexture(*texture);
                    delete texture;
                }
            }
        };

        RenderSystemData& GetRenderSystemData()
        {
            return drawableData;
        }

        static Camera2D*& GetMainCamera();

        void AddToRenderPool(alk::EntityId entityId, alk::TransformComponent* transform, alk::SpriteComponent* sprite);
        TextureHandle LoadRenderSystemTexture(std::string filename);

        static float CalculateSortKey(const Vector2 position);
    };
}