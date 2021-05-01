#include "stdafx.h"

#include "PlayerControlSystem.hpp"

#include "Components/PhysicBody.hpp"
#include "Components/RigidBody.hpp"
#include "Core/Entity.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <set>

void PlayerControlSystem::Update(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        RigidBody& rigidBody = world.GetComponent<RigidBody>(entity);
        PhysicBody& physicBody = world.GetComponent<PhysicBody>(entity);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            rigidBody.velocity.x = fmax(rigidBody.velocity.x - physicBody.speedInc, -physicBody.maxSpeed);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            rigidBody.velocity.x = fmin(rigidBody.velocity.x + physicBody.speedInc, physicBody.maxSpeed);
        } else {
            rigidBody.velocity.x *= physicBody.slowdownRate;
        }
    }
}