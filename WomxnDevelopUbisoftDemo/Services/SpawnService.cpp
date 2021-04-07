#include "stdafx.h"

#include "SpawnService.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Engine/EllipseShape.hpp"

void SpawnService::SpawnLevel(World& world, const sf::FloatRect& levelLimits)
{
    SpawnPlayer( // character
        world,
        sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, 100.f },
        sf::Vector2f { 100, 100 },
        sf::Color::Magenta);
    SpawnElement( // Left Edge
        world,
        sf::Vector2f { levelLimits.left, levelLimits.height / 2.f },
        sf::Vector2f { 10.f, levelLimits.height },
        sf::Color::Cyan);
    SpawnElement( // Right Edge
        world,
        sf::Vector2f { levelLimits.left + levelLimits.width, levelLimits.height / 2.f },
        sf::Vector2f { 10.f, levelLimits.height },
        sf::Color::Cyan);
    SpawnElement( // Ground
        world,
        sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, levelLimits.height },
        sf::Vector2f { 1500.f, 5.f },
        sf::Color::Yellow);
    SpawnElement( // platform
        world,
        sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, 700.f },
        sf::Vector2f { 200.f, 50.f },
        sf::Color::Cyan);
}

void SpawnService::SpawnPlayer(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color)
{
    Entity player = world.AddEntity();

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    sf::Transformable transform;
    transform.setPosition(center);
    sf::Transformable draftTransform;
    draftTransform.setPosition(center);

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);
    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    sf::CircleShape* shape = new sf::CircleShape { 50 };
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Transformable>(player, transform, draftTransform);
    world.AddComponentToEntity<Renderable>(player, shape, color, size, 1);
    world.AddComponentToEntity<RigidBody>(player, velocity, 400.f);
    world.AddComponentToEntity<PhysicBody>(player, 400.f, 30.f, 0.90f, 50.f, 10.f);
    world.AddComponentToEntity<CameraCenter>(player);
    world.AddComponentToEntity<Collideable>(player, boxCollideable, draftBoxCollideable);
}

void SpawnService::SpawnElement(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color)
{
    Entity platformEntity = world.AddEntity();

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);
    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);
    shape->setSize(size);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Renderable>(platformEntity, shape, color, size, 1);
    world.AddComponentToEntity<Collideable>(platformEntity, boxCollideable, draftBoxCollideable);
    world.AddComponentToEntity<RigidBody>(platformEntity, velocity, 0.f);
}

void SpawnService::SpawnLightDrop(World& world, const sf::FloatRect& levelLimits)
{
    Entity backgroundElement = world.AddEntity();

    int depth = GetRandomBetween(2, 5);
    int width = 3;
    int length = 20 * depth;
    int x = GetRandomBetween(levelLimits.left, levelLimits.width);
    int y = 0 - length;
    float speedTweak = 1 + length / 5.0f;
    float speed = 50 * speedTweak;

    sf::Vector2f position = sf::Vector2f { static_cast<float>(x), static_cast<float>(y) };
    sf::Vector2f size = sf::Vector2f { static_cast<float>(width), static_cast<float>(length) };
    sf::Color color = sf::Color { 140, 130, 215, 100 };
    sf::Color outlineColor = sf::Color { 140, 130, 215, 50 };
    sf::Vector2f velocity = sf::Vector2f { 0, speed };
    sf::Vector2f draftPosition = sf::Vector2f { 0, 0 };

    sf::Transformable transformable;
    transformable.setPosition(position);

    EllipseShape* shape = new EllipseShape { size };
    shape->setOrigin(size * 0.5f);
    shape->setPosition(position);

    shape->setFillColor(color);
    shape->setOutlineThickness(2);
    shape->setOutlineColor(outlineColor);

    world.AddComponentToEntity<Transformable>(backgroundElement, transformable, transformable);
    world.AddComponentToEntity<Renderable>(backgroundElement, shape, color, size, 2);
    world.AddComponentToEntity<RigidBody>(backgroundElement, velocity, 400.f);
}

int SpawnService::GetRandomBetween(int windowXMin, int windowXMax)
{
    return rand() % windowXMax + windowXMin;
}