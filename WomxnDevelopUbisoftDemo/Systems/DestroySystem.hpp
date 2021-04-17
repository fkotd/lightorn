#pragma once

#include "Core/World.hpp"

class DestroySystem : public System {
public:
    void DestroyOffScreen(World& world, sf::FloatRect levelLimits);
};
