#include "stdafx.h"

#include "App.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"

static const int APP_WINDOW_WIDTH { 1620 };
static const int APP_WINDOW_HEIGHT { 780 };
static constexpr float APP_MAX_FRAMERATE { 60.0f };
static const sf::Vector2u APP_WINDOW_SIZE { APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT };

App::App(const char* appName)
    : window { sf::VideoMode(APP_WINDOW_SIZE.x, APP_WINDOW_SIZE.y), appName, sf::Style::Titlebar | sf::Style::Close }
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
    window.setActive();

    sf::Vector2f levelTopLeft { 20.f / 100.f * APP_WINDOW_WIDTH, 0.f };
    sf::Vector2f levelSize { APP_WINDOW_WIDTH - (2.f * 20.f / 100.f * APP_WINDOW_WIDTH), 5.f * APP_WINDOW_HEIGHT };
    SetLevelLimits(levelTopLeft, levelSize);

    ImGui::SFML::Init(window);
}

App::~App()
{
    ImGui::SFML::Shutdown();
}

App& App::Build()
{
    RegisterComponents();
    RegisterSystems();

    return *this;
}

void App::Run()
{
    spawnService->SpawnEdge(*world, levelLimits);
    spawnService->SpawnGround(*world, levelLimits);
    spawnService->SpawnPlatform(*world, levelLimits);
    spawnService->SpawnPlayer(*world, levelLimits);

    float deltaTime { 1.0f / APP_MAX_FRAMERATE };
    sf::Clock clock;
    sf::Clock backgroundClock;
    sf::Time backgroundSpawnInterval = sf::milliseconds(50);

    while (window.isOpen()) {

        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;
            }
            }

            ImGui::SFML::ProcessEvent(event);
        }

        ImGui::SFML::Update(window, clock.restart());

        // TODO : move background element spawning in a function
        if (backgroundClock.getElapsedTime().asMilliseconds() >= backgroundSpawnInterval.asMilliseconds()) {
            spawnService->SpawnBackground(*world, levelLimits);
            backgroundClock.restart();
        }

        playerControlSystem->Update(*world, deltaTime);
        transformSystem->Update(*world, deltaTime);
        physicSystem->Update(*world, deltaTime);
        collisionSystem->Update(*world);
        renderSystem->Render(*world, window);

        ImGui::EndFrame();
        ImGui::SFML::Render(window);

        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
    }
}

void App::RegisterComponents()
{
    world->RegisterComponent<CameraCenter>();
    world->RegisterComponent<Collideable>();
    world->RegisterComponent<PhysicBody>();
    world->RegisterComponent<Renderable>();
    world->RegisterComponent<RigidBody>();
    world->RegisterComponent<Transformable>();
}

void App::RegisterSystems()
{
    collisionSystem = world->RegisterSystem<CollisionSystem, Collideable, RigidBody>();

    physicSystem = world->RegisterSystem<PhysicSystem, Transformable, RigidBody, PhysicBody>();
    physicSystem->SetGravity(sf::Vector2f({ 0.0f, 100.f }));

    playerControlSystem = world->RegisterSystem<PlayerControlSystem, RigidBody, PhysicBody>();

    renderSystem = world->RegisterSystem<RenderSystem, Renderable>();

    transformSystem = world->RegisterSystem<TransformSystem, Transformable, RigidBody>();
}

void App::SetLevelLimits(const sf::Vector2f& topLeft, const sf::Vector2f& size)
{
    levelLimits.left = topLeft.x;
    levelLimits.top = topLeft.y;
    levelLimits.width = size.x;
    levelLimits.height = size.y;
}