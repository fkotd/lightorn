#include <stdafx.h>
#include <iostream>

#include "Entity.hpp"
#include "Position.hpp"
#include "Renderable.hpp"
#include "RenderSystem.hpp"
#include "Signature.hpp"
#include "System.hpp"
#include "World.hpp"
#include <Game/GameDemo.h>

static constexpr float APP_MAX_FRAMERATE{ 60.0f };
static const sf::Vector2u APP_INIT_WINDOW_SIZE{ 1024, 768 };

int main()
{
	// GameDemo game{};
	// game.RunGameLoop();

	World world{};

	// Create entities

	Entity groundEntity = world.AddEntity();

	Signature groundEntitySignature = world.GetEntitySignature(groundEntity);
	if (groundEntitySignature == NULL) {
		groundEntitySignature = -1;
	}
	std::cout << "Create ground entity (id = " << groundEntity << " signature = " << groundEntitySignature << ")\n";

	Entity playerEntity = world.AddEntity();

	// Register components

	world.RegisterComponent<Position>();
	Component positionComponentId = world.GetComponent<Position>();

	std::cout << "Position component id = " << positionComponentId << "\n";

	world.RegisterComponent<Renderable>();
	Component renderableComponentId = world.GetComponent<Renderable>();

	std::cout << "Renderable component id = " << renderableComponentId << "\n";

	// Register systems

	std::shared_ptr<RenderSystem> renderSystem = world.RegisterSystem<RenderSystem>();

	Signature signature{};
	signature.set(world.GetComponent<Position>());
	signature.set(world.GetComponent<Renderable>());
	world.SetSystemSignature<RenderSystem>(signature);

	Signature renderSystemSignature = world.GetSystemSignature<RenderSystem>();
	std::cout << "RenderSystem bitset = " << renderSystemSignature << "\n";

	// Add components to entities

	// Initialize ground component attributes
	sf::Color color = sf::Color::Cyan;
	sf::Vector2f size = sf::Vector2f{ static_cast<float>(APP_INIT_WINDOW_SIZE.x), 100 };
	sf::Vector2f center = sf::Vector2f{ APP_INIT_WINDOW_SIZE.x / 2.0f, APP_INIT_WINDOW_SIZE.y - (size.y / 2.0f) };
	sf::RectangleShape rectangle = sf::RectangleShape{};
	rectangle.setSize(size);
	rectangle.setOrigin(size * 0.5f);
	rectangle.setPosition(center);
	rectangle.setFillColor(color);
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color{ static_cast<uint8_t>(color.r * 255.0f), static_cast<uint8_t>(color.g * 255.0f), static_cast<uint8_t>(color.b * 255.0f) });

	// Adding component

	world.AddComponent(groundEntity, Position{ center });

	groundEntitySignature = world.GetEntitySignature(groundEntity);
	std::cout << "Adding component position to ground entity (entity = " << groundEntity << " signature = " << groundEntitySignature << ")\n";

	Position positionComponent = world.GetComponent<Position>(groundEntity);
	std::cout << "Values of position component for the ground entity (x = " << positionComponent.position.x << " y = " << positionComponent.position.y << ")\n";
	
	world.AddComponent(groundEntity, Renderable{ &rectangle, color , size });
	Renderable renderable = world.GetComponent<Renderable>(groundEntity);
	std::cout << "Values of renderable component for the ground entity (width = " << renderable.size.x << " height = " << renderable.size.y << ")\n";

	// Initialize player attributes

	sf::Color playerColor = sf::Color::Magenta;
	sf::Vector2f playerSize = sf::Vector2f{ 100, 100 };
	sf::Vector2f playerCenter = sf::Vector2f{ APP_INIT_WINDOW_SIZE.x / 2.0f, APP_INIT_WINDOW_SIZE.y / 2.0f };
	sf::CircleShape playerShape = sf::CircleShape{50};
	//playerShape.setSize(playerSize);
	playerShape.setOrigin(playerSize * 0.5f);
	playerShape.setPosition(playerCenter);
	playerShape.setFillColor(playerColor);
	playerShape.setOutlineThickness(1);
	playerShape.setOutlineColor(sf::Color{ static_cast<uint8_t>(playerColor.r * 255.0f), static_cast<uint8_t>(playerColor.g * 255.0f), static_cast<uint8_t>(playerColor.b * 255.0f) });

	// Add components

	world.AddComponent(playerEntity, Position{ playerCenter });
	world.AddComponent(playerEntity, Renderable{ &playerShape, color, size });

	// Call systems

	sf::RenderWindow renderWindow{ sf::VideoMode(APP_INIT_WINDOW_SIZE.x, APP_INIT_WINDOW_SIZE.y), "Lightorn", sf::Style::Titlebar | sf::Style::Close };
	renderWindow.setVerticalSyncEnabled(true);
	renderWindow.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
	renderWindow.setActive();

	while (renderWindow.isOpen()) {
		renderSystem->Render(&world, renderWindow);
	}

	return EXIT_SUCCESS;
}