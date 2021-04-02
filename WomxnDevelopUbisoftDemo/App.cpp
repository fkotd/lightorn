#include "stdafx.h"

#include "App.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"

static constexpr float APP_MAX_FRAMERATE { 60.0f };
static const sf::Vector2u APP_WINDOW_SIZE { 1024, 1000 };

App::App(const char* appName)
    : window { sf::VideoMode(APP_WINDOW_SIZE.x, APP_WINDOW_SIZE.y), appName, sf::Style::Titlebar | sf::Style::Close }
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
    window.setActive();
}

App& App::Build()
{
    RegisterComponents();
    RegisterSystems();

    return *this;
}

void App::Run()
{
    spawnService->Spawn(*world);

    float deltaTime { 1.0f / APP_MAX_FRAMERATE };
    sf::Clock clock;

    while (window.isOpen()) {

        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        playerControlSystem->Update(*world, deltaTime);
        transformSystem->Update(*world, deltaTime);
        physicSystem->Update(*world, deltaTime);
        collisionSystem->Update(*world);

        renderSystem->Render(*world, &window);

        deltaTime = clock.getElapsedTime().asSeconds();
    }
}

void App::RegisterComponents()
{
    world->RegisterComponent<Transformable>();
    world->RegisterComponent<Renderable>();
    world->RegisterComponent<RigidBody>();
    world->RegisterComponent<CameraCenter>();
    world->RegisterComponent<Collideable>();
    world->RegisterComponent<PhysicBody>();
}

void App::RegisterSystems()
{
    renderSystem = world->RegisterSystem<RenderSystem, Renderable>();

    playerControlSystem = world->RegisterSystem<PlayerControlSystem, RigidBody, PhysicBody>();

    physicSystem = world->RegisterSystem<PhysicSystem, Transformable, RigidBody, PhysicBody>();
    physicSystem->SetGravity(sf::Vector2f({ 0.0f, 100.0f }));

    collisionSystem = world->RegisterSystem<CollisionSystem, Collideable, RigidBody>();

    transformSystem = world->RegisterSystem<TransformSystem, Transformable, RigidBody>();
}