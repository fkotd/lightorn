#pragma once

#include "System.hpp"
#include "Signature.hpp"

class SystemManager
{
public:
	template <typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* systemName = typeid(T).name();

		// TODO: Test that we do not register the same system more than once

		std::shared_ptr<T> system = std::make_shared<T>();
		systems.insert({systemName, system});

		return system;
	}

	template <typename T>
	void SetSystemSignature(Signature signature)
	{
		const char* systemName = typeid(T).name();

		auto it = systems.find(systemName);

		// TODO: check that we found the system

		it->second->SetSignature(signature);

	}

	template <typename T>
	Signature GetSystemSignature()
	{
		const char* systemName = typeid(T).name();

		auto it = systems.find(systemName);

		// TODO: check that we found the system

		return it->second->GetSignature();
	}

	/*void AddEntity(Entity entity, Signature entitySignature)
	{
		for (auto& system : systems) {
			if (AcceptEntity(entitySignature, system.second->GetSignature())) {
				system.second->AddEntity(entity);
				std::cout << "Added !" << "\n";
			}
		}
	}*/

	std::set<Entity> Find(std::unordered_map<Entity, Signature> entitiesSignature, Signature systemSignature)
	{
		std::set<Entity> entities{};

		for (auto& it : entitiesSignature) {
			if (AcceptEntity(it.second, systemSignature)) {
				entities.insert(it.first);
			}
		}

		return entities;
	}

	bool AcceptEntity(Signature entitySignature, Signature systemSignature)
	{
		// Each bit set to one in system signature must be set to one in entity signature
		return (entitySignature & systemSignature) == systemSignature;
	}

private:
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
};
