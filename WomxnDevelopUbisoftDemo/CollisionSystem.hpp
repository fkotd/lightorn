#pragma once

#include "System.hpp"
#include "World.hpp"

class CollisionSystem : public System
{
public:
	void Update(World* world);
};

