#pragma once

#include <memory>
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "Entity.hpp"
#include "Signature.hpp"

class World
{
public:
	World()
	{
		entityManager = std::make_unique<EntityManager>();
		componentManager = std::make_unique<ComponentManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	Entity AddEntity()
	{
		return entityManager->AddEntity();
	}

	Signature GetEntitySignature(Entity entity)
	{
		return componentManager->GetEntitySignature(entity);
	}

	template <typename T>
	void RegisterComponent()
	{
		componentManager->RegisterComponent<T>();
	}

	template <typename T>
	Component GetComponent()
	{
		return componentManager->GetComponent<T>();
	}

	template <typename T>
	T* GetComponent(Entity entity)
	{
		return componentManager->GetComponent<T>(entity);
	}

	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return systemManager->RegisterSystem<T>();
	}

	template <typename T>
	void SetSystemSignature(Signature signature)
	{
		systemManager->SetSystemSignature<T>(signature);
	}

	template <typename T>
	Signature GetSystemSignature()
	{
		return systemManager->GetSystemSignature<T>();
	}

	template <typename T>
	void AddComponent(Entity entity, T component)
	{
		componentManager->AddComponent(entity, component);
		
		// Signature entitySignature = componentManager->GetEntitySignature(entity);
		// systemManager->AddEntity(entity, entitySignature);
	}

	std::set<Entity> Find(Signature systemSignature)
	{
		return systemManager->Find(componentManager->GetEntitiesSignature(), systemSignature);
	}

private: 
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<SystemManager> systemManager;
};

