#include "stdafx.h"

#include "TransformSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"

void TransformSystem::Update(World& world, float deltaTime)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity);
        RigidBody& rigidBody = world.GetComponent<RigidBody>(entity);

        transformable.draftTransform.move(rigidBody.velocity * deltaTime);

        Collideable* collideable = world.GetComponentIfExists<Collideable>(entity);
        if (collideable != nullptr) {
            collideable->draftBoxCollideable.SetCenter(transformable.draftTransform.getPosition());
        }
    }
}