#pragma once

#include <unordered_map>
#include <memory>

#include "Component.hpp"
#include "ComponentData.hpp"
#include "Signature.hpp"

class ComponentManager
{
public:
	template <typename T>
	void RegisterComponent()
	{
		const char* componentName = typeid(T).name();

		// TODO: Test that we do not register the same type more than once
		// TODO: Test that we do not reach the maxmum number of components

		// Map component type name to id
		int componentId = componentIdsCounter++;
		componentIds.insert({componentName, componentId});

		// Create a map to hold component data
		componentsData.insert({ componentId, std::make_shared<ComponentData<T>>() });
	}

	template <typename T>
	Component GetComponent()
	{
		const char* componentName = typeid(T).name();
		auto it = componentIds.find(componentName);

		// TODO: check that we have found the component

		return it->second;
	}

	template <typename T>
	std::shared_ptr<ComponentData<T>> GetComponentData(Component componentType)
	{
		return std::static_pointer_cast<ComponentData<T>>(componentsData[componentType]);
	}

	template <typename T>
	T* GetComponent(Entity entity)
	{
		Component componentType = GetComponent<T>();


		std::shared_ptr<ComponentData<T>> componentData = std::static_pointer_cast<ComponentData<T>>(componentsData[componentType]);
		// std::shared_ptr<ComponentData<T>> componentData = GetComponentData(componentType);

		return componentData->GetComponent(entity);
	}

	Signature GetEntitySignature(Entity entity)
	{
		auto it = entitiesSignature.find(entity);
		if (it == entitiesSignature.end()) {
			return NULL;
		}

		return it->second;
	}

	template <typename T>
	Signature GetComponentSignature()
	{
		Component component = GetComponent<T>();
		
		Signature signature{};
		signature.set(component);

		return signature;
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
			// TODO : log component not found
			return;
		}

		// This is the first component of the entity
		// TODO : move that in another function
		if (entitySignatureIterator == entitiesSignature.end()) {
			// TODO : log entity not found
			Signature signature;
			// TODO : log the new signature
			entitiesSignature.insert({entity, signature});
			entitySignatureIterator = entitiesSignature.find(entity);
		}
		
		Component componentType = componentIdIterator->second;
		
		entitySignatureIterator->second.set(componentType);

		// Add the compoment to the component data
		std::shared_ptr<ComponentData<T>> componentData = std::static_pointer_cast<ComponentData<T>>(componentsData[componentType]);
		// std::shared_ptr<ComponentData<T>> componentData = GetComponentData(componentType);
		componentData->AddComponent(entity, component);
	}

	std::unordered_map<Entity, Signature> GetEntitiesSignature()
	{
		return entitiesSignature;
	}

private:
	std::unordered_map<const char*, Component> componentIds{};
	Component componentIdsCounter = 0;

	std::unordered_map<Entity, Signature> entitiesSignature{};

	std::unordered_map<Component, std::shared_ptr<IComponentData>> componentsData{};
};


