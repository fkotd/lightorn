#include "stdafx.h"

#include "TransformSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Core/Entity.hpp"
#include <set>

void TransformSystem::Update(World& world, float deltaTime)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity);
        RigidBody& rigidBody = world.GetComponent<RigidBody>(entity);

        transformable.draftTransformable.setPosition(transformable.transformable.getPosition() + rigidBody.velocity * deltaTime);

        Collideable* collideable = world.GetComponentIfExists<Collideable>(entity);
        if (collideable != nullptr) {
            collideable->draftBoxCollideable.SetCenter(transformable.draftTransformable.getPosition());
        }
    }
}