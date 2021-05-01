#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"
#include <SFML/System/Vector2.hpp>

class PhysicSystem : public System {
public:
    PhysicSystem();
    void Update(World& world, float deltaTime);

private:
    sf::Vector2f gravity;
};