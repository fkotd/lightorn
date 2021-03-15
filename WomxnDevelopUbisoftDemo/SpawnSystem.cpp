#include "stdafx.h"
#include "SpawnSystem.hpp"
#include "Transformable.hpp"
#include "Renderable.hpp"
#include "RigidBody.hpp"

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
	sf::Transformable transformable{};
	transformable.setPosition(center);
	sf::CircleShape* shape = new sf::CircleShape{ 50 };
	//playerShape.setSize(playerSize);
	shape->setOrigin(size * 0.5f);
	shape->setPosition(center);
	shape->setFillColor(color);
	shape->setOutlineThickness(1);
	shape->setOutlineColor(sf::Color{ static_cast<uint8_t>(color.r * 255.0f), static_cast<uint8_t>(color.g * 255.0f), static_cast<uint8_t>(color.b * 255.0f) });

	world->AddComponentToEntity(player, Transformable{ transformable });
	world->AddComponentToEntity(player, Renderable{ shape, color, size });
	world->AddComponentToEntity(player, RigidBody{ velocity });

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
	shape->setOutlineColor(sf::Color{ static_cast<uint8_t>(color.r * 255.0f), static_cast<uint8_t>(color.g * 255.0f), static_cast<uint8_t>(color.b * 255.0f) });

	//world->AddComponent(groundEntity, Position{ center });
	world->AddComponentToEntity(groundEntity, Renderable{ shape, color , size });
}