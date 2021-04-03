#pragma once

#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Signature.hpp"
#include "SystemManager.hpp"
#include <memory>

class World {
public:
    Entity AddEntity()
    {
        return entityManager.AddEntity();
    }

    template <typename T>
    void RegisterComponent()
    {
        componentManager.RegisterComponent<T>();
    }

    template <typename T>
    Component GetComponent() const
    {
        return componentManager.GetComponent<T>();
    }

    template <typename T>
    T& GetComponent(Entity entity)
    {
        return componentManager.GetComponent<T>(entity);
    }

    template <typename T>
    T* GetComponentIfExists(Entity entity)
    {
        return componentManager.GetComponentIfExists<T>(entity);
    }

    template <typename T, typename... Components>
    std::shared_ptr<T> RegisterSystem()
    {
        auto system = systemManager.RegisterSystem<T>();

        Signature systemSignature;
        (systemSignature.set(GetComponent<Components>()), ...);

        SetSystemSignature<T>(systemSignature);

        return system;
    }

    template <typename T>
    void SetSystemSignature(Signature signature)
    {
        systemManager.SetSystemSignature<T>(signature);
    }

    template <typename T, typename... Attributes>
    void AddComponentToEntity(Entity entity, Attributes... attributes)
    {
        componentManager.AddComponent(entity, T { attributes... });
    }

    std::set<Entity> Find(Signature searchedSignature) const
    {
        return componentManager.Find(searchedSignature);
    }

private:
    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;
};