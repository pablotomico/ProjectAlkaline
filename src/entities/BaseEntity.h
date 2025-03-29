#pragma once

#include <typeindex>
#include <unordered_map>

#include "include/raylib.h"

#include "systems/GameLogic.h"
#include "components/BaseComponent.h"

/**
 * @brief BaseEntity
 */
class BaseEntity
{
public:
    /**
     * @brief unique identifier
     * 
     * @todo generate automatically via register
     */
    unsigned short id;

    /**
     * @brief Readable name of the entity (dev-only)
     * 
     * @todo generate base on class + id (?)
     */
    const char *name;

    BaseEntity() : id(0), name("None") {}

    /**
     * @brief Checks if it's a valid entity reference
     *
     * @return true
     * @return false
     */
    bool IsValid()
    {
        return id > 0;
    }

    /**
     * @brief Returns the entity name - debug purposes only
     * 
     * @return const char* 
     */
    const char * GetName()
    {
        return name;
    }

private:
    /**
     * @brief Map of components - each component can be retrieved by type (using std::type_index)
     */
    std::unordered_map<std::type_index, std::shared_ptr<BaseComponent>> components;

public:

    /**
     * @brief Adds a component by the component type
     * Uses variadic arguments to support component default constructors and constructors with parameters
     * Forwarding magic to preserve value types of parameters
     * @example obj.AddComponent<BaseComponent>();
     * @todo throw error if component doesn't exist
     * @todo Check if component compatible
     * @return component
     */
    template <typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args) {
        std::type_index type = std::type_index(typeid(T));

        // Return the component if it already exists
        if (HasComponent<T>()) {
            return std::dynamic_pointer_cast<T>(components[type]);
        }
    
        // Create and add the new component
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        components[type] = comp;

        alk::GameLogic::NotifyCallbacks<T>(comp.get());
        return comp;
    }

    /**
     * @brief Get a component by type (if it exists)
     * @example obj.GetComponent<BaseComponent>();
     * @todo throw error if component doesn't exist
     * @return component|nullptr
     */
    template <typename T>
    std::shared_ptr<T> GetComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (HasComponent<T>()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    /**
     * @brief Remove a component by type (if it exists)
     * Returns true if successful
     * @example obj.GetComponent<BaseComponent>();
     * @todo throw error if component doesn't exist
     * @return bool
     */
    template <typename T>
    bool RemoveComponent() {
        if (HasComponent<T>())
        {
            components.erase(std::type_index(typeid(T)));
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if an entity has a component of a given type
     * @example obj.HasComponent<BaseComponent>();
     * @return bool
     */
    template <typename T>
    bool HasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    void Update(float deltaTime) {
        for (auto& [key, component] : components) {
            component->Update(deltaTime);
        }
    }
};