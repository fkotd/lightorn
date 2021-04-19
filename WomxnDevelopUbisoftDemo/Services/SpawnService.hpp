#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void SpawnLevel(World&, const sf::FloatRect& levelLimits);
    void SpawnLightDrop(World& world, const sf::FloatRect& levelLimits);
    void SpawnLightBall(World& world, const sf::FloatRect& levelLimits);

private:
    Entity SpawnPlayer(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
    Entity SpawnHeart(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
    Entity SpawnGround(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
    Entity SpawnElement(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
};
