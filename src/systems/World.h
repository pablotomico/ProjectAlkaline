#pragma once

#include <format>
#include <unordered_map>
#include <typeindex>

#include "alkaline_lib.h"
#include "entities/Entity.h"

namespace alk
{
    struct EntityMeta
    {
        EntityId id;
        std::string name;
        bool valid = true;

        EntityMeta() = default;
        EntityMeta(EntityId id, std::string name) : id(id), name(name) {}
    };

    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void Remove(EntityId entity) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        void Add(EntityId entity, T component)
        {
            if (entityIndices.find(entity) != entityIndices.end())
            {
                return;
            }

            entityIndices[entity] = components.size();
            components.push_back(std::move(component));
            entities.push_back(entity);
        }

        T* Get(EntityId entity)
        {
            auto it = entityIndices.find(entity);
            if (it == entityIndices.end())
            {
                return nullptr;
            }
            else
            {
                return &components[it->second];
            }
        }

        bool Has(EntityId entity)
        {
            return entityIndices.find(entity) != entityIndices.end();
        }

        void Remove(EntityId entity)
        {
            auto it = entityIndices.find(entity);
            if (it == entityIndices.end())
            {
                return;
            }

            size_t indexToRemove = it->second;
            size_t lastIndex = components.size() - 1;

            if (indexToRemove != lastIndex)
            {
                components[indexToRemove] = std::move(components[lastIndex]);
                entities[indexToRemove] = entities[lastIndex];
                entityIndices[entities[indexToRemove]] = indexToRemove;
            }

            components.pop_back();
            entities.pop_back();
            entityIndices.erase(it);
        }

        std::vector<T> components;
        std::vector<EntityId> entities;
        std::unordered_map<EntityId, size_t> entityIndices;
    };

    class World
    {
    public:
        Entity CreateEntity()
        {
            EntityId id = ++nextEntityId;
            entities[id] = EntityMeta{id, std::format("New Entity({})", id)};
            return Entity{id, entities[id].name.c_str()};
        }

        Entity CreateEntity(const char *name)
        {
            EntityId id = ++nextEntityId;
            entities[id] = EntityMeta{id, name};
            return Entity{id, name};
        }

        Entity GetEntity(EntityId id)
        {
            if (entities.find(id) != entities.end())
            {
                return Entity{id, entities[id].name};
            }
            else
            {
                return Entity{};
            }
        }

        void DestroyEntity(Entity &entity)
        {
            ALK_ASSERT(IsValid(entity), "World::DestroyEntity entity is not valid!");
            for (auto& [type, array] : componentArrays)
            {
                array->Remove(entity.id);
            }
            entities[entity.id].valid = false;
        }

        template <typename T, typename... Args>
        void AddComponent(Entity &entity, Args &&...args)
        {
            GetOrCreateComponentArray<T>()->Add(entity.id, std::move(T(std::forward<Args>(args)...)));
        }

        template <typename T>
        void RemoveComponent(Entity &entity)
        {
            GetOrCreateComponentArray<T>()->Remove(entity.id);
        }

        template <typename T>
        T* GetComponent(Entity &entity)
        {
            return GetOrCreateComponentArray<T>()->Get(entity.id);
        }

        template <typename T>
        bool HasComponent(EntityId id)
        {
            return GetOrCreateComponentArray<T>()->Has(id);
        }

        bool IsValid(Entity &entity)
        {
            return (entities.find(entity.id) != entities.end()) && entities[entity.id].valid;
        }

        template <typename T>
        ComponentArray<T>* GetComponents()
        {
            return GetOrCreateComponentArray<T>();
        }

    private:
        alk::EntityId nextEntityId = 0;
        std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> componentArrays;
        std::unordered_map<EntityId, EntityMeta> entities;

        template <typename T>
        ComponentArray<T> *GetOrCreateComponentArray()
        {
            std::type_index index = std::type_index(typeid(T));
            if (componentArrays.find(index) == componentArrays.end())
            {
                componentArrays[index] = std::make_unique<ComponentArray<T>>();
            }
            return static_cast<ComponentArray<T> *>(componentArrays[index].get());
        }
    };
}