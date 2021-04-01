#include "stdafx.h"

#include "PhysicSystem.hpp"
#include "RigidBody.hpp"
#include "Transformable.hpp"

void PhysicSystem::SetGravity(sf::Vector2f gravity)
{
    this->gravity = gravity;
}

void PhysicSystem::Update(const std::unique_ptr<World>& world, float deltaTime)
{
    std::set<Entity> entities = world->Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world->GetComponent<Transformable>(entity); // move it into the transformable system ?
        RigidBody& rigidBody = world->GetComponent<RigidBody>(entity);

        transformable.transformable.move(rigidBody.velocity * deltaTime);
        rigidBody.velocity += gravity * deltaTime;
    }
}