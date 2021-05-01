#pragma once

#include "Core/Entity.hpp"
#include "Core/System.hpp"
#include "Core/World.hpp"
#include "Tools/Layer.hpp"
#include <set>

namespace sf {
class RenderTarget;
}

class RenderSystem : public System {
public:
    void Render(World& world, sf::RenderTarget& target);

private:
    void RenderLayer(World& world, sf::RenderTarget& target, const std::set<Entity>& entities, Layer layer);
};