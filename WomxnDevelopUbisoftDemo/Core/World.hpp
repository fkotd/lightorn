#pragma once

#include "Component.hpp"
#include "ComponentManager.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Event.hpp"
#include "EventManager.hpp"
#include "Signature.hpp"
#include "SystemManager.hpp"
#include <memory>
#include <set>
#include <string>

class World {
public:
    Entity AddEntity()
    {
        return entityManager.AddEntity();
    }

    void RemoveEntity(Entity entity)
    {
        componentManager.RemoveEntity(entity);
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

    template <typename... Components>
    std::unique_ptr<Signature> GetSignature()
    {
        auto signature = std::make_unique<Signature>();
        (signature->set(GetComponent<Components>()), ...);

        return signature;
    }

    void AddGameEvent(const std::string& name, Event event)
    {
        eventManager.AddGameEvent(name, event);
    }

    void RemoveGameEvent(const std::string& name)
    {
        eventManager.RemoveGameEvent(name);
    }

    Event* GetGameEvent(const std::string& name)
    {
        return eventManager.GetGameEvent(name);
    }

    void ClearEvents()
    {
        eventManager.ClearEvents();
    }

    void ClearAllEvents()
    {
        eventManager.ClearAllEvents();
    }

private:
    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;
    EventManager eventManager;
};
