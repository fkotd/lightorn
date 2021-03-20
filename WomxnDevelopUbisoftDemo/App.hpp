#pragma once

#include "CollisionSystem.hpp"
#include "PlayerControlSystem.hpp"
#include "PhysicSystem.hpp"
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
	std::shared_ptr<PhysicSystem> physicSystem;
	std::shared_ptr<CollisionSystem> collisionSystem;
};

