#include "stdafx.h"
#include "CollisionSystem.hpp"
#include "Collideable.hpp"
#include "RigidBody.hpp"

void CollisionSystem::Update(const std::unique_ptr<World>& world)
{
	std::set<Entity> entities = world->Find(GetSignature());

	for (auto entity1 : entities) {
		Collideable* collideable1 = world->GetComponent<Collideable>(entity1);

		// TODO: loop in way that entities are not checked twice
		for (auto entity2 : entities) {
			if (entity1 != entity2) {
				Collideable* collideable2 = world->GetComponent<Collideable>(entity2);

				if (collideable1->boxCollideable->IsColliding(*collideable2->boxCollideable)) {

					// Get the collision direction
					sf::Vector2f collisionNorm = collideable1->boxCollideable->GetNormalizedCollisionVetor(*collideable2->boxCollideable);

					RigidBody* rigidBody1 = world->GetComponent<RigidBody>(entity1);
					RigidBody* rigidBody2 = world->GetComponent<RigidBody>(entity2);

					sf::Vector2f relativeVelocity = sf::Vector2f(rigidBody1->velocity.x - rigidBody2->velocity.x, rigidBody1->velocity.y - rigidBody2->velocity.y);
					float speed = relativeVelocity.x * collisionNorm.x + relativeVelocity.y * collisionNorm.y;

					if (speed < 0) {
						break;
					}

					//rigidBody1->velocity.x -= (speed * collisionNorm.x);
					//rigidBody1->velocity.y -= (speed * collisionNorm.y);
					//rigidBody2->velocity.x += (speed * collisionNorm.x);
					//rigidBody2->velocity.y += (speed * collisionNorm.y);

					rigidBody1->velocity.x = 0;
					rigidBody1->velocity.y = 0;
					rigidBody2->velocity.x = 0;
					rigidBody2->velocity.y = 0;

					std::cout << "Colliding !!" << std::endl;
				}
			}
		}
	}
}