#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void SpawnPlayer(World& world, const sf::FloatRect levelLimits);
    void SpawnPlatform(World& world, const sf::FloatRect levelLimits);
    void SpawnGround(World& world, const sf::FloatRect levelLimits);
    void SpawnEdge(World& world, const sf::FloatRect levelLimits);
    void SpawnBackground(World& world, const sf::FloatRect levelLimits);

private:
    int GetRandomBetween(int windowXMin, int windowXMax);
};
