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
		systems.emplace(systemName, system);

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
	Signature GetSystemSignature() const
	{
		const char* systemName = typeid(T).name();

		auto it = systems.find(systemName);

		// TODO: check that we found the system

		return it->second->GetSignature();
	}

private:
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
};
