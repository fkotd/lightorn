#pragma once

#include "Entity.hpp"
#include <unordered_map>

template <typename T>
class ComponentData {
public:
    void AddComponent(Entity entity, T component)
    {
        componentData.emplace(entity, component);
    }

    T& GetComponent(Entity entity)
    {
        auto it = componentData.find(entity);

        return it->second;
    }

    T* GetComponentIfExists(Entity entity)
    {
        auto it = componentData.find(entity);

        if (it == componentData.end()) {
            return nullptr;
        }

        return &(it->second);
    }

private:
    std::unordered_map<Entity, T> componentData;
};