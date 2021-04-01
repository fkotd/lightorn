#include "stdafx.h"

#include "PhysicBody.hpp"
#include "PlayerControlSystem.hpp"
#include "RigidBody.hpp"

void PlayerControlSystem::Update(const std::unique_ptr<World>& world, float deltaTime)
{
    std::set<Entity> entities = world->Find(GetSignature());

    for (auto entity : entities) {
        RigidBody& rigidBody = world->GetComponent<RigidBody>(entity);
        PhysicBody& physicBody = world->GetComponent<PhysicBody>(entity);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            rigidBody.velocity.x = fmax(rigidBody.velocity.x - physicBody.speedInc, -physicBody.maxSpeed);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            rigidBody.velocity.x = fmin(rigidBody.velocity.x + physicBody.speedInc, physicBody.maxSpeed);
        } else {
            rigidBody.velocity.x *= physicBody.slowdownRate;
        }
    }
}