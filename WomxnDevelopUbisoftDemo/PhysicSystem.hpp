#pragma once

#include "System.hpp"
#include "World.hpp"

class PhysicSystem : public System {
public:
    void SetGravity(sf::Vector2f gravity);
    void Update(const std::unique_ptr<World>& world, float deltaTime);

private:
    sf::Vector2f gravity;
};