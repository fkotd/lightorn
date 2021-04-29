#pragma once

#include "Core/World.hpp"

class PhysicSystem : public System {
public:
    void SetGravity(sf::Vector2f newGravity);
    void Update(World& world, float deltaTime);

private:
    sf::Vector2f gravity;
};