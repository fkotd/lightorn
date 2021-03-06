#pragma once

#include "Core/World.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class SpawnService {
public:
    SpawnService();
    void SpawnLevel(World&, const sf::FloatRect& levelLimits);
    void SpawnLightDrop(World& world, const sf::FloatRect& levelLimits, const sf::Color& color);
    void SpawnLightBall(World& world, const sf::FloatRect& levelLimits);

private:
    void SpawnLimitElements(World& world, const sf::FloatRect& levelLimits);
    void SpawnPlatforms(World& world, const sf::FloatRect& levelLimits);
    void SpawnPlateform(World& world, const sf::FloatRect& levelLimits, float yMin, float yMax, float probability);
    Entity SpawnPlatform(World& world, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Vector2f& spriteOriginFactor, const sf::Vector2f& spriteScaleFactor, const sf::Color& color, bool isShapeVisible);
    Entity SpawnCharacter(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnHeart(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnGround(World& world, const sf::FloatRect& levelLimits);
    Entity SpawnEdge(World& world, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Color& color, bool isShapeVisible);
};
