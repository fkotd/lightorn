#pragma once

#include <SFML/System/Vector2.hpp>

struct RigidBody {
    sf::Vector2f velocity;
    float maxFallVelocity;
};