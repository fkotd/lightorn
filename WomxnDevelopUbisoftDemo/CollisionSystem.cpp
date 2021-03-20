#include "stdafx.h"
#include "CollisionSystem.hpp"
#include "Collideable.hpp"

void CollisionSystem::Update(World* world)
{
	std::set<Entity> entities = world->Find(GetSignature());

	for (auto entity1 : entities) {
		Collideable* collideable1 = world->GetComponent<Collideable>(entity1);

		// TODO: loop in way that entities are not checked twice
		for (auto entity2 : entities) {
			if (entity1 != entity2) {
				Collideable* collideable2 = world->GetComponent<Collideable>(entity2);

				if (collideable1->boxCollideable->IsColliding(*collideable2->boxCollideable)) {
					std::cout << "Colliding !!" << std::endl;
				}
			}
		}
	}
}