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
	T* GetComponent(Entity entity)
	{
		return componentManager.GetComponent<T>(entity);
	}

	template <typename T>
	T* GetComponentIfExists(Entity entity)
	{
		return componentManager.GetComponentIfExists<T>(entity);
	}

	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return systemManager.RegisterSystem<T>();
	}

	template <typename T>
	void SetSystemSignature(Signature signature)
	{
		systemManager.SetSystemSignature<T>(signature);
	}

	template <typename T>
	void AddComponentToEntity(Entity entity, T component)
	{
		componentManager.AddComponent(entity, component);
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

