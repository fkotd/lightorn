#pragma once

#include "System.hpp"
#include "World.hpp"

class PhysicSystem : public System
{
	void Update(World* world, float deltaTime);
};

