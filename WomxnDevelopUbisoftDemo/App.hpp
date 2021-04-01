#pragma once

#include "CollisionSystem.hpp"
#include "PhysicSystem.hpp"
#include "PlayerControlSystem.hpp"
#include "RenderSystem.hpp"
#include "SpawnService.hpp"
#include "TransformSystem.hpp"
#include "World.hpp"
#include <memory>

class App {
public:
    App(const char* appName);
    App& Build();
    void Run();

private:
    void RegisterComponents();
    void RegisterSystems();

    sf::RenderWindow window;

    std::unique_ptr<World> world = std::make_unique<World>();

    std::shared_ptr<SpawnService> spawnService;

    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<PlayerControlSystem> playerControlSystem;
    std::shared_ptr<PhysicSystem> physicSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<TransformSystem> transformSystem;
};
