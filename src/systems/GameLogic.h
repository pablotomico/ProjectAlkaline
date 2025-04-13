#pragma once

#include <vector>
#include <functional>
#include <typeindex>

#include "alkaline_lib.h"

class BaseComponent;

template <typename T>
using Callback = std::function<void(T*)>;

namespace alk
{
    namespace GameLogic
    {
        class GameLogicSystem;

        inline std::vector<GameLogicSystem *>& GetSystems()
        {
            static std::vector<GameLogicSystem *> subsystems;
            return subsystems;
        }

        template <typename T>
        inline static std::unordered_map<std::type_index, std::vector<Callback<T>>> callbacks;

        inline void AddSystem(GameLogicSystem *system)
        {
            GetSystems().emplace_back(system);
        }

        template <typename T>
        inline void SubscribeToComponent(Callback<T> callback)
        {
            std::type_index type = std::type_index(typeid(T));

            callbacks<T>[type].emplace_back(callback);
        }

        template <typename T>
        inline void NotifyCallbacks(T *obj)
        {
            std::type_index type = std::type_index(typeid(T));
            for (Callback<T> callback : callbacks<T>[type])
            {
                callback(obj);
            }
        }

        void Initialize();
        void Update(const float deltaTime);
    }
}