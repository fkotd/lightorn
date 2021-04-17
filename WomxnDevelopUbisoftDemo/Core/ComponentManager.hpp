#pragma once

#include "Component.hpp"
#include "ComponentData.hpp"
#include "ComponentInfo.hpp"
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
        std::shared_ptr<ComponentData<T>> componentData = std::make_shared<ComponentData<T>>();
        componentsData.emplace(componentId, componentData);

        // Initialize callback to remove entity data when needed
        ComponentInfo componentInfo;
        componentInfo.RemoveEntityCallBack = [componentData](Entity entity) { componentData->RemoveComponent(entity); };
        componentsInfo.emplace(componentId, componentInfo);
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

    void RemoveEntity(Entity entity)
    {
        RemoveComponents(entity);
        RemoveSignature(entity);
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

    void RemoveComponents(Entity entity)
    {
        // TODO: can we find a way to optimize ?

        // Find all components of the entity

        // Get the entity signature
        Signature signature = entitiesSignature.at(entity);
        // Iterate over all bit set to one
        for (int componentId = 0; componentId < MAX_COMPONENTS; componentId++) {
            if (signature[componentId] == 1) {
                componentsInfo.at(componentId).RemoveEntityCallBack(entity);
            }
        }
    }

    //template <typename T>
    void RemoveComponentData(Entity entity, Component component)
    {
        //componentsData.at(component).RemoveComponent(entity);
    }

    void RemoveSignature(Entity entity)
    {
        entitiesSignature.erase(entity);
    }

    Component componentIdsCounter = 0;
    std::unordered_map<const char*, Component> componentIds;
    std::unordered_map<Entity, Signature> entitiesSignature;
    std::unordered_map<Component, std::any> componentsData;
    std::unordered_map<Component, ComponentInfo> componentsInfo;
};
