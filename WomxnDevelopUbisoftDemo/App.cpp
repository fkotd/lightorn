#include "stdafx.h"

#include "App.hpp"
#include "Components/Animation.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Dynamic.hpp"
#include "Components/Fatal.hpp"
#include "Components/Feel.hpp"
#include "Components/Grippable.hpp"
#include "Components/Gripper.hpp"
#include "Components/Mortal.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Reborner.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Sprite.hpp"
#include "Components/Static.hpp"
#include "Components/Transformable.hpp"
#include "Tools/Messages.hpp"
#include "Tools/Random.hpp"

static const int APP_WINDOW_WIDTH { 1620 };
static const int APP_WINDOW_HEIGHT { 780 };
static constexpr float APP_MAX_FRAMERATE { 60.0f };
static const sf::Vector2u APP_WINDOW_SIZE { APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT };

App::App(const char* appName)
    : window { sf::VideoMode(APP_WINDOW_SIZE.x, APP_WINDOW_SIZE.y), appName, sf::Style::Titlebar | sf::Style::Close }
    , startScreen { "Press X to start" }
    , isLevelStared { false }
    , isLevelEnded { false }
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
    window.setActive();

    sf::Vector2f levelTopLeft { 20.f / 100.f * APP_WINDOW_WIDTH, 0.f };
    sf::Vector2f levelSize { APP_WINDOW_WIDTH - (2.f * 20.f / 100.f * APP_WINDOW_WIDTH), 2.f * APP_WINDOW_HEIGHT };
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
    sf::Time lightDropSpawnInterval = sf::milliseconds(GetRandomIntBetween(50, 100));
    sf::Time lightBallSpawnInterval = sf::seconds(static_cast<float>(GetRandomIntBetween(1, 1)));
    sf::Time animationInterval = sf::milliseconds(500);
    sf::Music music;

    if (!music.openFromFile("Assets/main_theme.wav")) {
        return;
    }
    music.setLoop(true);
    music.play();

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

        window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Update(window, clock.restart());

        if (!isLevelStared && !isLevelEnded) {
            DisplayStartScreen();
        } else {
            DisplayLevelScreen(lightDropClock, lightBallClock, animationClock, lightDropSpawnInterval, lightBallSpawnInterval, animationInterval, deltaTime);
        }
        //else if (isLevelStared && !isLevelEnded) {
        //    DisplayLevelScreen(lightDropClock, lightBallClock, animationClock, lightDropSpawnInterval, lightBallSpawnInterval, animationInterval, deltaTime);
        //}
        //else if (isLevelStared && isLevelEnded) {
        //    DisplayEndScreen();
        //}

        ImGui::EndFrame();
        ImGui::SFML::Render(window);
        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
        world->ClearEvents();
    }

    world->ClearAllEvents();
    music.stop();
}

void App::DisplayStartScreen()
{
    startScreen.DrawScreen(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        isLevelStared = true;
    }
}

void App::DisplayLevelScreen(sf::Clock& lightDropClock, sf::Clock& lightBallClock, sf::Clock& animationClock, sf::Time lightDropSpawnInterval, sf::Time lightBallSpawnInterval, sf::Time animationInterval, float deltaTime)
{
    // TODO : move in a function
    if (lightDropClock.getElapsedTime().asMilliseconds() >= lightDropSpawnInterval.asMilliseconds()) {
        spawnService->SpawnLightDrop(*world, levelLimits);
        lightDropClock.restart();
        lightDropSpawnInterval = sf::milliseconds(GetRandomIntBetween(50, 100));
    }

    // TODO : move in a function
    if (lightBallClock.getElapsedTime().asSeconds() >= lightBallSpawnInterval.asSeconds()) {
        spawnService->SpawnLightBall(*world, levelLimits);
        lightBallClock.restart();
        lightBallSpawnInterval = sf::seconds(static_cast<float>(GetRandomIntBetween(1, 1)));
    }

    playerControlSystem->Update(*world, deltaTime);
    physicSystem->Update(*world, deltaTime);
    transformSystem->Update(*world, deltaTime);
    gripSystem->Update(*world, levelLimits);
    feelSystem->Update(*world, deltaTime);
    collisionSystem->Update(*world, deltaTime);
    commitSystem->Commit(*world);

    if (animationClock.getElapsedTime().asMilliseconds() >= animationInterval.asMilliseconds()) {
        animationSystem->Update(*world);
        animationClock.restart();
    }

    renderSystem->Render(*world, window);
    destroySystem->DestroyOffScreen(*world, levelLimits);

    if (IsLevelEndedByDeath()) {
        isLevelEnded = true;
        ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Game Over");
    } else if (IsLevelEndedByReborn()) {
        isLevelEnded = true;
        ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Reborn");
    }
}

void App::DisplayEndScreen()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        isLevelStared = true;
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
    world->RegisterComponent<RigidBody>();
    world->RegisterComponent<Dynamic>();
    world->RegisterComponent<Static>();
    world->RegisterComponent<Transformable>();
    world->RegisterComponent<Animation>();
    world->RegisterComponent<Reborner>();
    world->RegisterComponent<Sprite>();
    world->RegisterComponent<Feel>();
}

void App::RegisterSystems()
{
    playerControlSystem = world->RegisterSystem<PlayerControlSystem, RigidBody, PhysicBody>();
    physicSystem = world->RegisterSystem<PhysicSystem, Transformable, RigidBody, PhysicBody>();
    physicSystem->SetGravity(sf::Vector2f({ 0.0f, 100.f }));
    transformSystem = world->RegisterSystem<TransformSystem, Transformable, RigidBody>();
    collisionSystem = world->RegisterSystem<CollisionSystem, Collideable, Static>();
    commitSystem = world->RegisterSystem<CommitSystem, Transformable>();
    renderSystem = world->RegisterSystem<RenderSystem, Renderable>();
    gripSystem = world->RegisterSystem<GripSystem, Grippable, Collideable, Transformable>();
    animationSystem = world->RegisterSystem<AnimationSystem, Animation, Sprite>();
    destroySystem = world->RegisterSystem<DestroySystem, Transformable>();
    feelSystem = world->RegisterSystem<FeelSystem, Grippable, Feel, Sprite>();
}

void App::SetLevelLimits(const sf::Vector2f& topLeft, const sf::Vector2f& size)
{
    levelLimits.left = topLeft.x;
    levelLimits.top = topLeft.y;
    levelLimits.width = size.x;
    levelLimits.height = size.y;
}

bool App::IsLevelEndedByDeath()
{
    Event* deathEvent = world->GetGameEvent(END_GAME_DEATH);
    if (deathEvent != nullptr) {
        try {
            return std::any_cast<bool>(deathEvent->GetValue());
        } catch (const std::bad_any_cast& _) {
        }
    }
    return false;
}

bool App::IsLevelEndedByReborn()
{
    Event* rebornEvent = world->GetGameEvent(END_GAME_REBORN);
    if (rebornEvent != nullptr) {
        try {
            return std::any_cast<bool>(rebornEvent->GetValue());
        } catch (const std::bad_any_cast& _) {
        }
    }
    return false;
}
