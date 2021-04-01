#pragma once

#include "Core/World.hpp"

class RenderSystem : public System {
public:
    void Render(const std::unique_ptr<World>& world, sf::RenderWindow* window);
};