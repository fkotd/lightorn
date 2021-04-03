#pragma once

#include "Component.hpp"
#include "ComponentData.hpp"
#include "Entity.hpp"
#include "Signature.hpp"
#include <any>
#include <memory>
#include <unordered_map>

class ComponentManager {
public:
    template <typename T>
    void RegisterComponent()
    {
        const char* componentName = typeid(T).name();

        // TODO: Test that we do not register the same type more than once
        // TODO: Test that we do not reach the maxmum number of components

        // Map component type name to id
        int componentId = componentIdsCounter++;
        componentIds.emplace(componentName, componentId);

        // Create a map to hold component data
        componentsData.emplace(componentId, std::make_shared<ComponentData<T>>());
    }

    template <typename T>
    Component GetComponent() const
    {
        const char* componentName = typeid(T).name();
        auto it = componentIds.find(componentName);

        // TODO: check that we have found the component

        return it->second;
    }

    template <typename T>
    T& GetComponent(Entity entity)
    {
        Component componentType = GetComponent<T>();

        auto componentData = GetComponentData<T>(componentType);

        return componentData->GetComponent(entity);
    }

    template <typename T>
    T* GetComponentIfExists(Entity entity)
    {
        Component componentType = GetComponent<T>();

        auto componentData = GetComponentData<T>(componentType);

        return componentData->GetComponentIfExists(entity);
    }

    template <typename T>
    void AddComponent(Entity entity, T component)
    {
        // Update entity signature
        const char* componentName = typeid(T).name();
        auto componentIdIterator = componentIds.find(componentName);
        auto entitySignatureIterator = entitiesSignature.find(entity);

        // The component does not exists or hasn't been registered
        if (componentIdIterator == componentIds.end()) {
            // TODO: log component not found
            return;
        }

        // This is the first component of the entity
        // TODO: move that in another function
        if (entitySignatureIterator == entitiesSignature.end()) {
            // TODO: log entity not found
            Signature signature;
            // TODO: log the new signature
            entitiesSignature.emplace(entity, signature);
            entitySignatureIterator = entitiesSignature.find(entity);
        }

        Component componentType = componentIdIterator->second;

        entitySignatureIterator->second.set(componentType);

        // Add the compoment to the component data
        auto componentData = GetComponentData<T>(componentType);

        componentData->AddComponent(entity, component);
    }

    std::set<Entity> Find(Signature searchedSignature) const
    {
        std::set<Entity> entities;

        for (auto& it : entitiesSignature) {
            if (AcceptEntity(it.second, searchedSignature)) {
                entities.insert(it.first);
            }
        }

        return entities;
    }

private:
    bool AcceptEntity(Signature entitySignature, Signature searchedSignature) const
    {
        // Each bit set to one in system signature must be set to one in entity signature
        return (entitySignature & searchedSignature) == searchedSignature;
    }

    template <typename T>
    std::shared_ptr<ComponentData<T>> GetComponentData(Component componentType)
    {
        return *std::any_cast<std::shared_ptr<ComponentData<T>>>(&componentsData[componentType]);
    }

    Component componentIdsCounter = 0;
    std::unordered_map<const char*, Component> componentIds;
    std::unordered_map<Entity, Signature> entitiesSignature;
    std::unordered_map<Component, std::any> componentsData;
};