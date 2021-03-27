#include "stdafx.h"
#include "PlayerControlSystem.hpp"
#include "Transformable.hpp"
#include "Renderable.hpp"
#include "RigidBody.hpp"

static const float SPEED_MAX = 400.0f;
static const float SPEED_INC = 30.0f;
static const float SLOWDOWN_RATE = 0.90f;

void PlayerControlSystem::Update(const std::unique_ptr<World>& world, float deltaTime)
{
	std::set<Entity> entities = world->Find(GetSignature());

	for (auto entity : entities) {
		Transformable& transformable = world->GetComponent<Transformable>(entity);
		RigidBody& rigidBody = world->GetComponent<RigidBody>(entity);
		Renderable& renderable = world->GetComponent<Renderable>(entity);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			rigidBody.velocity.x = fmax(rigidBody.velocity.x - SPEED_INC, -SPEED_MAX);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			rigidBody.velocity.x = fmin(rigidBody.velocity.x + SPEED_INC, SPEED_MAX);
		}
		else {
			rigidBody.velocity.x *= SLOWDOWN_RATE;
		}
		
		transformable.transformable.move(rigidBody.velocity * deltaTime);
	}
}