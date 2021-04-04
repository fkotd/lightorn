#pragma once

#include "Core/World.hpp"
#include "Services/SpawnService.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/PhysicSystem.hpp"
#include "Systems/PlayerControlSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/TransformSystem.hpp"
#include <memory>

class App {
public:
    App(const char* appName);
    ~App();
    App& Build();
    void Run();

private:
    void RegisterComponents();
    void RegisterSystems();

    sf::RenderWindow window;

    std::unique_ptr<World> world = std::make_unique<World>();

    std::unique_ptr<SpawnService> spawnService = std::make_unique<SpawnService>();

    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<PhysicSystem> physicSystem;
    std::shared_ptr<PlayerControlSystem> playerControlSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<TransformSystem> transformSystem;
};
