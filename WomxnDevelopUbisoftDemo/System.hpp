#pragma once

#include "Entity.hpp"
#include "Signature.hpp"
#include <set>

class System
{
public:
	void SetSignature(Signature signature)
	{
		this->signature = signature;
	}

	Signature GetSignature()
	{
		return this->signature;
	}

	void AddEntity(Entity entity)
	{
		entities.insert(entity);
	}

protected:
	std::set<Entity> entities;

private:
	Signature signature;
};
