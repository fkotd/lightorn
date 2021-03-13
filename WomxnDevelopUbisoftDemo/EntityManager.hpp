#pragma once

#include <iostream>
#include <queue>
#include "Entity.hpp"

class EntityManager
{
public:
	EntityManager()
	{
		for (int entity = 0; entity < MAX_ENTITIES; entity++) {
			// std::cout << "create new entity of id = " << entity << "\n";
			entitiesPool.push(entity);
		}
	}

	Entity AddEntity()
	{
		if (entitiesPool.empty()) {
			return NULL;
		}

		Entity entity = entitiesPool.front();
		entitiesPool.pop();

		return entity;
	}

private:
	std::queue<Entity> entitiesPool{};
};
