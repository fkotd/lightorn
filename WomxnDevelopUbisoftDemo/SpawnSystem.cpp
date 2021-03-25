#include "stdafx.h"
#include "SpawnSystem.hpp"
#include "Transformable.hpp"
#include "Renderable.hpp"
#include "RigidBody.hpp"
#include "CameraCenter.hpp"
#include "Collideable.hpp"

void SpawnSystem::Spawn(World* world)
{
	SpawnPlayer(world);
	SpawnElement(world);
}

void SpawnSystem::SpawnPlayer(World* world)
{
	Entity player = world->AddEntity();

	sf::Color color = sf::Color::Magenta;
	sf::Vector2f size = sf::Vector2f{ 100, 100 };
	sf::Vector2f center = sf::Vector2f{ 1024 / 2.0f, 768 / 2.0f };
	sf::Vector2f velocity = sf::Vector2f{ 0, 0 };
	sf::Transformable transformable;
	transformable.setPosition(center);
	sf::CircleShape* shape = new sf::CircleShape{ 50 };
	shape->setOrigin(size * 0.5f);
	shape->setPosition(center);
	shape->setFillColor(color);
	shape->setOutlineThickness(1);
	shape->setOutlineColor(color);
	BoxCollideable* boxCollideable = new BoxCollideable{};
	boxCollideable->SetBoundingBox(center, size);

	world->AddComponentToEntity(player, Transformable{ transformable });
	world->AddComponentToEntity(player, Renderable{ shape, color, size });
	world->AddComponentToEntity(player, RigidBody{ velocity });
	world->AddComponentToEntity(player, CameraCenter{});
	world->AddComponentToEntity(player, Collideable{ boxCollideable });
}

void SpawnSystem::SpawnElement(World* world)
{
	Entity groundEntity = world->AddEntity();

	sf::Color color = sf::Color::Cyan;
	sf::Vector2f size = sf::Vector2f{ 1024, 100 };
	sf::Vector2f center = sf::Vector2f{ 1024 / 2.0f, 768 - (size.y / 2.0f) };
	sf::RectangleShape* shape = new sf::RectangleShape();
	shape->setSize(size);
	shape->setOrigin(size * 0.5f);
	shape->setPosition(center);
	shape->setFillColor(color);
	shape->setOutlineThickness(1);
	shape->setOutlineColor(color);
	BoxCollideable* boxCollideable = new BoxCollideable{};
	boxCollideable->SetBoundingBox(center, size);
	sf::Vector2f velocity = sf::Vector2f{ 0, 0 };

	//world->AddComponent(groundEntity, Position{ center });
	world->AddComponentToEntity(groundEntity, Renderable{ shape, color , size });
	world->AddComponentToEntity(groundEntity, Collideable{ boxCollideable });
	world->AddComponentToEntity(groundEntity, RigidBody{ velocity });

}