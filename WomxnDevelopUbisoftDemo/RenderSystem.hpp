#pragma once

#include "World.hpp"

class RenderSystem : public System
{
public:
	void Render(World* world, sf::RenderWindow* window);
};