#include "stdafx.h"

#include "TransformSystem.hpp"

#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"

void TransformSystem::Update(const std::unique_ptr<World>& world, float deltaTime)
{
    std::set<Entity> entities = world->Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world->GetComponent<Transformable>(entity);
        RigidBody& rigidBody = world->GetComponent<RigidBody>(entity);

        transformable.transformable.move(rigidBody.velocity * deltaTime);
    }
}