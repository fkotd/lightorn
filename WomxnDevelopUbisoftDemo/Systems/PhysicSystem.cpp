#include "stdafx.h"

#include "PhysicSystem.hpp"

#include "Components/PhysicBody.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"

#include <cmath>

void PhysicSystem::SetGravity(sf::Vector2f gravity)
{
    this->gravity = gravity;
}

void PhysicSystem::Update(World& world, float deltaTime)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity); // move it into the transformable system ?
        RigidBody& rigidBody = world.GetComponent<RigidBody>(entity);
        PhysicBody& physicBody = world.GetComponent<PhysicBody>(entity);

        //rigidBody.velocity += ((physicBody.mass * gravity) / physicBody.gamma) * (1 - exp((-physicBody.gamma * deltaTime) / physicBody.mass));

        rigidBody.velocity += gravity * deltaTime;

        if (rigidBody.velocity.y > rigidBody.maxFallVelocity) {
            rigidBody.velocity.y = rigidBody.maxFallVelocity;
        }

        ImGui::Begin("Physic Menu");
        ImGui::Text("Character velocity: %f, %f", rigidBody.velocity.x, rigidBody.velocity.y);
        ImGui::End();
    }
}