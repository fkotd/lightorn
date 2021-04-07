#pragma once

#include "Core/World.hpp"

class CollisionSystem : public System {
public:
    void Update(World& world);

private:
    float SweptAABB(const sf::FloatRect& r1, const sf::FloatRect& r2, sf::Vector2f& v1, sf::Vector2f& v2, sf::Vector2f& normal);
};
