#pragma once

#include "System.hpp"
#include "World.hpp"

class PhysicSystem : public System
{
public:
	void SetGravity(sf::Vector2f gravity);
	void Update(World* world, float deltaTime);

private:
	sf::Vector2f gravity;
};

