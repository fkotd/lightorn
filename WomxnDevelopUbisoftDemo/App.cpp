#include "stdafx.h"

#include "App.hpp"
#include "Components/Animation.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Darkness.hpp"
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
    , startScreen { "Press X to start", "Assets/start_screen.png" }
    , gameOverScreen { "Game over...", "Assets/end_screen.png" }
    , rebornScreen { "Thank you for playing", "Assets/end_screen.png" }
    , isLevelStared { false }
    , isLevelEndedByDeath { false }
    , isLevelEndedByReborn { false }
{
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
    window.setActive();

    initialView = window.getView();

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

        if (!isLevelStared && !isLevelEndedByDeath && !isLevelEndedByReborn) {
            DisplayStartScreen();
        }
        //else {
        //    DisplayLevelScreen(lightDropClock, lightBallClock, animationClock, lightDropSpawnInterval, lightBallSpawnInterval, animationInterval, deltaTime);
        //}
        else if (isLevelStared && !isLevelEndedByDeath && !isLevelEndedByReborn) {
            DisplayLevelScreen(lightDropClock, lightBallClock, animationClock, lightDropSpawnInterval, lightBallSpawnInterval, animationInterval, deltaTime);
        } else if (isLevelStared && isLevelEndedByDeath) {
            DisplayGameOverScreen();
        } else if (isLevelStared && isLevelEndedByReborn) {
            DisplayRebornScreen();
        }

        ImGui::EndFrame();
        ImGui::SFML::Render(window);
        window.display();

        deltaTime = clock.getElapsedTime().asSeconds();
        world->ClearEvents();
    }

    world->ClearAllEvents();
    music.stop();
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

    playerControlSystem->Update(*world);
    physicSystem->Update(*world, deltaTime);
    gripSystem->Update(*world, levelLimits);
    transformSystem->Update(*world, deltaTime);
    feelSystem->Update(*world);
    collisionSystem->Update(*world, deltaTime);
    commitSystem->Commit(*world);
    darknessSystem->Update(*world);

    if (animationClock.getElapsedTime().asMilliseconds() >= animationInterval.asMilliseconds()) {
        animationSystem->Update(*world);
        animationClock.restart();
    }

    renderSystem->Render(*world, window);
    destroySystem->DestroyOffScreen(*world, levelLimits);

    if (IsLevelEndedByDeath()) {
        isLevelEndedByDeath = true;
        ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Game Over");
    } else if (IsLevelEndedByReborn()) {
        isLevelEndedByReborn = true;
        ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Reborn");
    }
}

// TODO: factorize
void App::DisplayStartScreen()
{
    startScreen.Draw(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        isLevelStared = true;
    }
}

// TODO: factorize
void App::DisplayGameOverScreen()
{
    window.setView(initialView);
    gameOverScreen.Draw(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        isLevelStared = true;
    }
}

// TODO: factorize
void App::DisplayRebornScreen()
{
    window.setView(initialView);
    rebornScreen.Draw(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        isLevelStared = true;
    }
}

void App::RegisterComponents()
{
    world->RegisterComponent<Feel>();
    world->RegisterComponent<Fatal>();
    world->RegisterComponent<Sprite>();
    world->RegisterComponent<Mortal>();
    world->RegisterComponent<Static>();
    world->RegisterComponent<Dynamic>();
    world->RegisterComponent<Gripper>();
    world->RegisterComponent<Reborner>();
    world->RegisterComponent<Darkness>();
    world->RegisterComponent<Grippable>();
    world->RegisterComponent<RigidBody>();
    world->RegisterComponent<Animation>();
    world->RegisterComponent<Renderable>();
    world->RegisterComponent<PhysicBody>();
    world->RegisterComponent<Collideable>();
    world->RegisterComponent<CameraCenter>();
    world->RegisterComponent<Transformable>();
}

void App::RegisterSystems()
{
    feelSystem = world->RegisterSystem<FeelSystem, Grippable, Feel, Sprite>();
    gripSystem = world->RegisterSystem<GripSystem, Grippable, Collideable, RigidBody>();
    renderSystem = world->RegisterSystem<RenderSystem, Renderable>();
    commitSystem = world->RegisterSystem<CommitSystem, Transformable>();
    physicSystem = world->RegisterSystem<PhysicSystem, Transformable, RigidBody, PhysicBody>();
    destroySystem = world->RegisterSystem<DestroySystem, Transformable>();
    darknessSystem = world->RegisterSystem<DarknessSystem, Darkness, Transformable, RigidBody>();
    animationSystem = world->RegisterSystem<AnimationSystem, Animation, Sprite>();
    collisionSystem = world->RegisterSystem<CollisionSystem, Collideable, Static>();
    transformSystem = world->RegisterSystem<TransformSystem, Transformable, RigidBody>();
    playerControlSystem = world->RegisterSystem<PlayerControlSystem, RigidBody, PhysicBody>();
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
        return deathEvent->GetValue<bool>();
    }
    return false;
}

bool App::IsLevelEndedByReborn()
{
    Event* rebornEvent = world->GetGameEvent(END_GAME_REBORN);
    if (rebornEvent != nullptr) {
        return rebornEvent->GetValue<bool>();
    }
    return false;
}
