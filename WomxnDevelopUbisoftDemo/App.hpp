#pragma once

#include "Core/World.hpp"
#include "Services/SpawnService.hpp"
#include "Systems/AnimationSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/CommitSystem.hpp"
#include "Systems/DarknessSystem.hpp"
#include "Systems/DestroySystem.hpp"
#include "Systems/FeelSystem.hpp"
#include "Systems/GripSystem.hpp"
#include "Systems/PhysicSystem.hpp"
#include "Systems/PlayerControlSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/TransformSystem.hpp"
#include "Tools/Feeling.hpp"
#include "UI/Screen.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
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
    void SetLevelLimits(const sf::Vector2f& levelTopLeft, const sf::Vector2f& levelSize);
    void DisplayLevelScreen(sf::Clock& lightDropClock, sf::Clock& lightBallClock, sf::Clock& animationClock, sf::Time lightDropSpawnInterval, sf::Time lightBallSpawnInterval, sf::Time animationInterval, float deltaTime);
    void DisplayUIScreen(Screen screen);
    bool IsLevelEnded(const std::string& eventName) const;
    sf::Color GetGrippedFeelingColor() const;

    sf::View initialView;
    Screen startScreen;
    Screen gameOverScreen;
    Screen rebornScreen;
    bool isLevelStared;
    bool isLevelEndedByDeath;
    bool isLevelEndedByReborn;

    sf::RenderWindow window;
    sf::FloatRect levelLimits;

    std::unique_ptr<World> world = std::make_unique<World>();

    std::unique_ptr<SpawnService> spawnService = std::make_unique<SpawnService>();

    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<PhysicSystem> physicSystem;
    std::shared_ptr<PlayerControlSystem> playerControlSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<TransformSystem> transformSystem;
    std::shared_ptr<CommitSystem> commitSystem;
    std::shared_ptr<GripSystem> gripSystem;
    std::shared_ptr<AnimationSystem> animationSystem;
    std::shared_ptr<DestroySystem> destroySystem;
    std::shared_ptr<FeelSystem> feelSystem;
    std::shared_ptr<DarknessSystem> darknessSystem;
};
