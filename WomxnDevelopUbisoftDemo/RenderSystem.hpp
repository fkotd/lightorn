#pragma once

#include "System.hpp"
#include "World.hpp"

class RenderSystem : public System
{
public:
	void Render(World* world, sf::RenderWindow& window);
};

