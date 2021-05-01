#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"
#include <SFML/Graphics/Rect.hpp>

class DestroySystem : public System {
public:
    void DestroyOffScreen(World& world, sf::FloatRect levelLimits);
};
