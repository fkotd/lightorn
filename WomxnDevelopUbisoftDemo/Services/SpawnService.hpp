#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    SpawnService();
    void SpawnLevel(World&, const sf::FloatRect& levelLimits);
    void SpawnLightDrop(World& world, const sf::FloatRect& levelLimits);
    void SpawnLightBall(World& world, const sf::FloatRect& levelLimits);

private:
    void SpawnLimitElements(World& world, const sf::FloatRect& levelLimits);
    void SpawnLevelElements(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnCharacter(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnHeart(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnGround(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnElement(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
};
