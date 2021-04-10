#include "stdafx.h"

#include "CollisionSystem.hpp"

#include "Components/Collideable.hpp"

void CollisionSystem::Update(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    int i = 0;
    int j = 0;

    for (auto iEntity : entities) {
        Collideable& iCollideable = world.GetComponent<Collideable>(iEntity);

        for (auto jEntity : entities) {
            if (j > i) {
                Collideable& jCollideable = world.GetComponent<Collideable>(jEntity);

                if (iCollideable.draftBoxCollideable.IsColliding(jCollideable.draftBoxCollideable)) {
                    world.AddCollisionEvent(iEntity, jEntity);
                }
            }
            j++;
        }
        i++;
    }
}
