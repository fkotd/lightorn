#include "stdafx.h"

#include "SpawnService.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Engine/EllipseShape.hpp"

void SpawnService::Spawn(World& world)
{
    SpawnBackground(world, 10);
    SpawnPlayer(world);
    SpawnElement(world);
}

void SpawnService::SpawnPlayer(World& world)
{
    Entity player = world.AddEntity();

    sf::Color color = sf::Color::Magenta;
    sf::Vector2f size = sf::Vector2f { 100, 100 };
    sf::Vector2f center = sf::Vector2f { 1024 / 2.0f, 768 / 2.0f };
    sf::Vector2f velocity = sf::Vector2f { 0, 0 };
    sf::Transformable transformable;
    transformable.setPosition(center);
    sf::CircleShape* shape = new sf::CircleShape { 50 };
    shape->setOrigin(size * 0.5f);
    shape->setPosition(center);
    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);
    BoxCollideable* boxCollideable = new BoxCollideable {};
    boxCollideable->SetBoundingBox(center, size);

    world.AddComponentToEntity<Transformable>(player, transformable);
    world.AddComponentToEntity<Renderable>(player, shape, color, size);
    world.AddComponentToEntity<RigidBody>(player, velocity);
    world.AddComponentToEntity<PhysicBody>(player, 400.0f, 30.0f, 0.90f);
    world.AddComponentToEntity<CameraCenter>(player);
    world.AddComponentToEntity<Collideable>(player, boxCollideable);
}

void SpawnService::SpawnElement(World& world)
{
    Entity groundEntity = world.AddEntity();

    sf::Color color = sf::Color::Cyan;
    sf::Vector2f size = sf::Vector2f { 1024, 100 };
    sf::Vector2f center = sf::Vector2f { 1024 / 2.0f, 768 - (size.y / 2.0f) };
    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setSize(size);
    shape->setOrigin(size * 0.5f);
    shape->setPosition(center);
    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);
    BoxCollideable* boxCollideable = new BoxCollideable {};
    boxCollideable->SetBoundingBox(center, size);
    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    world.AddComponentToEntity<Renderable>(groundEntity, shape, color, size);
    world.AddComponentToEntity<Collideable>(groundEntity, boxCollideable);
    world.AddComponentToEntity<RigidBody>(groundEntity, velocity);
}

void SpawnService::SpawnBackground(World& world, int numberOfElements)
{
    for (int i = 0; i < numberOfElements; i++) {
        Entity backgroundElement = world.AddEntity();

        sf::Vector2f center = sf::Vector2f { static_cast<float>(50 * i), 10 };

        sf::Transformable transformable;
        transformable.setPosition(center);

        sf::Color color = sf::Color { 140, 130, 215, 100 };

        sf::Vector2f size = sf::Vector2f { 50, 50 };

        EllipseShape* shape = new EllipseShape { sf::Vector2f { 3, 25 } };
        shape->setOrigin(size * 0.5f);
        shape->setPosition(center);
        shape->setFillColor(color);
        shape->setOutlineThickness(1);
        shape->setOutlineColor(color);

        sf::Vector2f velocity = sf::Vector2f { 0, 400 };

        world.AddComponentToEntity<Transformable>(backgroundElement, transformable);
        world.AddComponentToEntity<Renderable>(backgroundElement, shape, color, size);
        world.AddComponentToEntity<RigidBody>(backgroundElement, velocity);
    }
}