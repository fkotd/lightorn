#pragma once

#include "Core/World.hpp"

class RenderSystem : public System {
public:
    void Render(World& world, sf::RenderTarget& target);

private:
    void RenderLayer(World& world, sf::RenderTarget& target, const std::set<Entity>& entities, int layer);
};