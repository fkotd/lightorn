#include "stdafx.h"

#include "SpawnService.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Engine/EllipseShape.hpp"

void SpawnService::SpawnPlayer(World& world, const sf::FloatRect levelLimits)
{
    Entity player = world.AddEntity();

    sf::Color color = sf::Color::Magenta;
    sf::Vector2f size = sf::Vector2f { 100, 100 };
    sf::Vector2f center = sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, 100.f };
    sf::Vector2f velocity = sf::Vector2f { 0, 0 };
    sf::Transformable transformable;
    transformable.setPosition(center);
    sf::CircleShape* shape = new sf::CircleShape { 50 };
    shape->setOrigin(size * 0.5f);
    shape->setPosition(center);
    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);
    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);

    world.AddComponentToEntity<Transformable>(player, transformable);
    world.AddComponentToEntity<Renderable>(player, shape, color, size);
    world.AddComponentToEntity<RigidBody>(player, velocity);
    world.AddComponentToEntity<PhysicBody>(player, 400.f, 30.f, 0.90f, 50.f, 10.f);
    world.AddComponentToEntity<CameraCenter>(player);
    world.AddComponentToEntity<Collideable>(player, boxCollideable);
}

void SpawnService::SpawnPlatform(World& world, const sf::FloatRect levelLimits)
{
    Entity platformEntity = world.AddEntity();

    float width = 200.f;
    float height = 50.f;
    float x = ((2 * levelLimits.left) + levelLimits.width) / 2.0f;
    float y = 700;

    sf::Vector2f position = sf::Vector2f { x, y };
    sf::Vector2f size = sf::Vector2f { width, height };
    sf::Color color = sf::Color::Cyan;

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setSize(size);
    shape->setOrigin(size * 0.5f);
    shape->setPosition(position);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(position, size);

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    world.AddComponentToEntity<Renderable>(platformEntity, shape, color, size);
    world.AddComponentToEntity<Collideable>(platformEntity, boxCollideable);
    world.AddComponentToEntity<RigidBody>(platformEntity, velocity);
}

void SpawnService::SpawnGround(World& world, const sf::FloatRect levelLimits)
{
    Entity groundEntity = world.AddEntity();

    float width = 1500.f;
    float height = 5.f;
    float x = ((2 * levelLimits.left) + levelLimits.width) / 2.0f;
    float y = levelLimits.height;

    sf::Vector2f position = sf::Vector2f { x, y };
    sf::Vector2f size = sf::Vector2f { width, height };
    sf::Color color = sf::Color::Yellow;

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setSize(size);
    shape->setOrigin(size * 0.5f);
    shape->setPosition(position);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(position, size);

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    world.AddComponentToEntity<Renderable>(groundEntity, shape, color, size);
    world.AddComponentToEntity<Collideable>(groundEntity, boxCollideable);
    world.AddComponentToEntity<RigidBody>(groundEntity, velocity);
}

void SpawnService::SpawnEdge(World& world, const sf::FloatRect levelLimits)
{
    float widthLimits[2] = { levelLimits.left, levelLimits.left + levelLimits.width };

    for (const auto x : widthLimits) {
        Entity limitEntity = world.AddEntity();

        float width = 10.f;
        float height = levelLimits.height;
        float y = levelLimits.height / 2.f;

        sf::Vector2f position = sf::Vector2f { x, y };
        sf::Vector2f size = sf::Vector2f { width, height };
        sf::Color color = sf::Color::Cyan;

        sf::RectangleShape* shape = new sf::RectangleShape();
        shape->setSize(size);
        shape->setOrigin(size * 0.5f);
        shape->setPosition(position);

        shape->setFillColor(color);
        shape->setOutlineThickness(1.f);
        shape->setOutlineColor(color);

        world.AddComponentToEntity<Renderable>(limitEntity, shape, color, size);
    }
}

void SpawnService::SpawnBackground(World& world, const sf::FloatRect levelLimits)
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

    sf::Transformable transformable;
    transformable.setPosition(position);

    EllipseShape* shape = new EllipseShape { size };
    shape->setOrigin(size * 0.5f);
    shape->setPosition(position);

    shape->setFillColor(color);
    shape->setOutlineThickness(2);
    shape->setOutlineColor(outlineColor);

    world.AddComponentToEntity<Transformable>(backgroundElement, transformable);
    world.AddComponentToEntity<Renderable>(backgroundElement, shape, color, size);
    world.AddComponentToEntity<RigidBody>(backgroundElement, velocity);
}

int SpawnService::GetRandomBetween(int windowXMin, int windowXMax)
{
    return rand() % windowXMax + windowXMin;
}