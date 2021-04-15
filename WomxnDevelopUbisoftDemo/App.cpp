#include "stdafx.h"

#include "App.hpp"
#include "Components/Animation.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Fatal.hpp"
#include "Components/Grippable.hpp"
#include "Components/Gripper.hpp"
#include "Components/Mortal.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Renderable.hpp"
#include "Components/Responser.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Tools/Random.hpp"

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
    //sf::Vector2f levelSize { APP_WINDOW_WIDTH - (2.f * 20.f / 100.f * APP_WINDOW_WIDTH), 5.f * APP_WINDOW_HEIGHT };
    sf::Vector2f levelSize { APP_WINDOW_WIDTH - (2.f * 20.f / 100.f * APP_WINDOW_WIDTH), 1.f * APP_WINDOW_HEIGHT };
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
    spawnService->SpawnLevel(*world, levelLimits);
    spawnService->SpawnLightBall(*world, levelLimits);

    float deltaTime { 1.0f / APP_MAX_FRAMERATE };
    sf::Clock clock;
    sf::Clock lightDropClock;
    sf::Clock lightBallClock;
    sf::Clock animationClock;
    sf::Time lightDropSpawnInterval = sf::milliseconds(GetRandomBetween(50, 100));
    sf::Time lightBallSpawnInterval = sf::seconds(static_cast<float>(GetRandomBetween(2, 5)));
    sf::Time animationInterval = sf::milliseconds(500);

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

        // TODO : move in a function
        if (lightDropClock.getElapsedTime().asMilliseconds() >= lightDropSpawnInterval.asMilliseconds()) {
            spawnService->SpawnLightDrop(*world, levelLimits);
            lightDropClock.restart();
            lightDropSpawnInterval = sf::milliseconds(GetRandomBetween(50, 100));
        }

        // TODO : move in a function
        if (lightBallClock.getElapsedTime().asSeconds() >= lightBallSpawnInterval.asSeconds()) {
            spawnService->SpawnLightBall(*world, levelLimits);
            lightBallClock.restart();
            lightBallSpawnInterval = sf::seconds(static_cast<float>(GetRandomBetween(2, 5)));
        }

        playerControlSystem->Update(*world, deltaTime);
        physicSystem->Update(*world, deltaTime);
        transformSystem->Update(*world, deltaTime);
        gripSystem->Update(*world, deltaTime);
        collisionSystem->Update(*world);
        commitSystem->Commit(*world);

        if (animationClock.getElapsedTime().asMilliseconds() >= animationInterval.asMilliseconds()) {
            animationSystem->Update(*world);
            animationClock.restart();
        }

        renderSystem->Render(*world, window);

        if (IsGameEnded()) {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "GAME ENDED");
        }

        ImGui::EndFrame();
        ImGui::SFML::Render(window);

        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
        world->ClearEvents();
    }
}

void App::RegisterComponents()
{
    world->RegisterComponent<CameraCenter>();
    world->RegisterComponent<Collideable>();
    world->RegisterComponent<Fatal>();
    world->RegisterComponent<Grippable>();
    world->RegisterComponent<Gripper>();
    world->RegisterComponent<Mortal>();
    world->RegisterComponent<PhysicBody>();
    world->RegisterComponent<Renderable>();
    world->RegisterComponent<Responser>();
    world->RegisterComponent<RigidBody>();
    world->RegisterComponent<Transformable>();
    world->RegisterComponent<Animation>();
}

void App::RegisterSystems()
{
    playerControlSystem = world->RegisterSystem<PlayerControlSystem, RigidBody, PhysicBody>();
    physicSystem = world->RegisterSystem<PhysicSystem, Transformable, RigidBody, PhysicBody>();
    physicSystem->SetGravity(sf::Vector2f({ 0.0f, 100.f }));
    transformSystem = world->RegisterSystem<TransformSystem, Transformable, RigidBody>();
    collisionSystem = world->RegisterSystem<CollisionSystem, Collideable, RigidBody, Responser>();
    commitSystem = world->RegisterSystem<CommitSystem, Transformable>();
    renderSystem = world->RegisterSystem<RenderSystem, Renderable>();
    gripSystem = world->RegisterSystem<GripSystem, Grippable, Collideable, Transformable>();
    animationSystem = world->RegisterSystem<AnimationSystem, Animation, Renderable>();
}

void App::SetLevelLimits(const sf::Vector2f& topLeft, const sf::Vector2f& size)
{
    levelLimits.left = topLeft.x;
    levelLimits.top = topLeft.y;
    levelLimits.width = size.x;
    levelLimits.height = size.y;
}
bool App::IsGameEnded()
{
    try {
        return std::any_cast<bool>(world->GetGameEvent("endgame"));
    } catch (const std::bad_any_cast& _) {
    }
    return false;
}