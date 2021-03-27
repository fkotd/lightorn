#pragma once

#include <memory>
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

	std::unique_ptr<World> world = std::make_unique<World>();

	std::shared_ptr<SpawnSystem> spawnSystem;
	std::shared_ptr<RenderSystem> renderSystem;
	std::shared_ptr<PlayerControlSystem> playerControlSystem;
	std::shared_ptr<PhysicSystem> physicSystem;
	std::shared_ptr<CollisionSystem> collisionSystem;
};

