#include "stdafx.h"
#include "PlayerControlSystem.hpp"
#include "Position.hpp"
#include "Renderable.hpp"

void PlayerControlSystem::Update(World* world)
{
	std::set<Entity> entities = world->Find(GetSignature());

	for (auto entity : entities) {
		Position* position = world->GetComponent<Position>(entity);
		Renderable* renderable = world->GetComponent<Renderable>(entity);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			// TODO : fix the name of the component positon
			position->position.x -= 10.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			position->position.x += 10.0f;
		}

		renderable->shape->setPosition(position->position);
	}
}