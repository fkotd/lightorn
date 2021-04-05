#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void SpawnLevel(World&, const sf::FloatRect& levelLimits);
    void SpawnLightDrop(World& world, const sf::FloatRect& levelLimits);

private:
    void SpawnPlayer(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
    void SpawnElement(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color);
    int GetRandomBetween(int windowXMin, int windowXMax);
};
