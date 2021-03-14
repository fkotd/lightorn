#pragma once

#include "PlayerControlSystem.hpp"
#include "RenderSystem.hpp"
#include "SpawnSystem.hpp"
#include "World.hpp"

class App
{
public:
	App(const char* appName);
	App& Build();
	void Run();
private:
	void RegisterComponents();
	void RegisterSystems();

	sf::RenderWindow window;
	World world;

	std::shared_ptr<SpawnSystem> spawnSystem;
	std::shared_ptr<RenderSystem> renderSystem;
	std::shared_ptr<PlayerControlSystem> playerControlSystem;
};

