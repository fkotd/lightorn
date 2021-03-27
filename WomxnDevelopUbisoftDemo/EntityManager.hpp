#pragma once

#include <iostream>
#include <queue>
#include "Entity.hpp"

class EntityManager
{
public:
	Entity AddEntity()
	{
		return entityIdsCounter++;
	}

private:
	Entity entityIdsCounter = 0;
};
