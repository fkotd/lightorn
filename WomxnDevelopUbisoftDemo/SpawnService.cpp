#include "stdafx.h"

#include "CameraCenter.hpp"
#include "Collideable.hpp"
#include "EllipseShape.hpp"
#include "PhysicBody.hpp"
#include "Renderable.hpp"
#include "RigidBody.hpp"
#include "SpawnService.hpp"
#include "Transformable.hpp"

void SpawnService::Spawn(const std::unique_ptr<World>& world)
{
    SpawnBackground(world, 10);
    SpawnPlayer(world);
    SpawnElement(world);
}

void SpawnService::SpawnPlayer(const std::unique_ptr<World>& world)
{
    Entity player = world->AddEntity();

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

    world->AddComponentToEntity(player, Transformable { transformable });
    world->AddComponentToEntity(player, Renderable { shape, color, size });
    world->AddComponentToEntity(player, RigidBody { velocity });
    world->AddComponentToEntity(player, PhysicBody { 400.0f, 30.0f, 0.90f });
    world->AddComponentToEntity(player, CameraCenter {});
    world->AddComponentToEntity(player, Collideable { boxCollideable });
}

void SpawnService::SpawnElement(const std::unique_ptr<World>& world)
{
    Entity groundEntity = world->AddEntity();

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

    //world->AddComponent(groundEntity, Position{ center });
    world->AddComponentToEntity(groundEntity, Renderable { shape, color, size });
    world->AddComponentToEntity(groundEntity, Collideable { boxCollideable });
    world->AddComponentToEntity(groundEntity, RigidBody { velocity });
}

void SpawnService::SpawnBackground(const std::unique_ptr<World>& world, int numberOfElements)
{
    for (int i = 0; i < numberOfElements; i++) {
        Entity backgroundElement = world->AddEntity();

        sf::Vector2f center = sf::Vector2f { static_cast<float>(50 * i), 10 };

        sf::Transformable transformable;
        transformable.setPosition(center);

        sf::Color color = sf::Color::Green;

        sf::Vector2f size = sf::Vector2f { 50, 50 };

        EllipseShape* shape = new EllipseShape { sf::Vector2f { 3, 25 } };
        shape->setOrigin(size * 0.5f);
        shape->setPosition(center);
        shape->setFillColor(color);
        shape->setOutlineThickness(1);
        shape->setOutlineColor(color);

        sf::Vector2f velocity = sf::Vector2f { 0, 0 };

        world->AddComponentToEntity(backgroundElement, Transformable { transformable });
        world->AddComponentToEntity(backgroundElement, Renderable { shape, color, size });
        world->AddComponentToEntity(backgroundElement, RigidBody { velocity });
    }
}