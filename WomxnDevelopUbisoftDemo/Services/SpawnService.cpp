#include "stdafx.h"

#include "SpawnService.hpp"

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
#include "Engine/EllipseShape.hpp"
#include "Tools/Layer.hpp"
#include "Tools/LoopMode.hpp"
#include "Tools/Random.hpp"

void SpawnService::SpawnLevel(World& world, const sf::FloatRect& levelLimits)
{
    SpawnPlayer( // Character
        world,
        sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, 100.f },
        sf::Vector2f { 50, 50 },
        sf::Color::Magenta);
    SpawnElement( // Left Edge
        world,
        sf::Vector2f { levelLimits.left, levelLimits.height / 2.f },
        sf::Vector2f { 10.f, levelLimits.height },
        sf::Color::Cyan);
    SpawnElement( // Right Edge
        world,
        sf::Vector2f { levelLimits.left + levelLimits.width, levelLimits.height / 2.f },
        sf::Vector2f { 10.f, levelLimits.height },
        sf::Color::Cyan);
    SpawnGround( // Ground
        world,
        sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, levelLimits.height },
        sf::Vector2f { (2 * levelLimits.left) + levelLimits.width, 5.f },
        sf::Color::Yellow);
    SpawnElement( // Platform
        world,
        sf::Vector2f { ((2 * levelLimits.left) + levelLimits.width) / 2.0f, 700.f },
        sf::Vector2f { 200.f, 50.f },
        sf::Color::Cyan);
}

void SpawnService::SpawnPlayer(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color)
{
    Entity player = world.AddEntity();

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    sf::Transformable transform;
    transform.setPosition(center);
    sf::Transformable draftTransform;
    draftTransform.setPosition(center);

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);
    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    sf::Texture* texture = new sf::Texture {};
    if (!texture->loadFromFile("Assets/eevy.png")) {
        std::cout << "Unable to load asset" << std::endl;
    }

    sf::Sprite* sprite = new sf::Sprite {};
    sf::Vector2f spriteOrigin { size.x / 3.1f, size.y / 2.2f };
    sprite->setTexture(*texture);
    sprite->setPosition(center);
    sprite->setOrigin(spriteOrigin);
    sprite->scale(2.9f, 2.9f);

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);
    shape->setSize(size);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Transformable>(player, transform, draftTransform);
    world.AddComponentToEntity<Renderable>(player, nullptr, color, size, Layer::Top, sprite);
    world.AddComponentToEntity<RigidBody>(player, velocity, 400.f);
    world.AddComponentToEntity<PhysicBody>(player, 400.f, 30.f, 0.90f, 50.f, 10.f);
    world.AddComponentToEntity<CameraCenter>(player);
    world.AddComponentToEntity<Collideable>(player, boxCollideable, draftBoxCollideable);
    world.AddComponentToEntity<Gripper>(player);
    world.AddComponentToEntity<Mortal>(player);
    world.AddComponentToEntity<Responser>(player);
}

void SpawnService::SpawnGround(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color)
{
    Entity ground = world.AddEntity();

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);
    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    sf::Texture* texture = new sf::Texture {};
    if (!texture->loadFromFile("Assets/ground_spritesheet.png")) {
        std::cout << "Unable to load asset" << std::endl;
    }
    texture->setRepeated(true);

    sf::Sprite* sprite = new sf::Sprite(*texture, sf::IntRect(0, 0, 64, 32));
    sf::Vector2f spriteOrigin { size.x / 20.f, size.y / 2.5f };
    sprite->setTextureRect(sf::IntRect(0, 0, 1000, 32));
    sprite->setOrigin(spriteOrigin);
    sprite->setPosition(center);
    sprite->scale(10.0f, 10.0f);

    std::map<int, sf::Vector2i> keyframes;
    keyframes[0] = sf::Vector2i(0, 0);
    keyframes[1] = sf::Vector2i(0, 32);
    keyframes[2] = sf::Vector2i(0, 64);

    sf::Vector2i texureSizeByFrame(1000, 32);

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);
    shape->setSize(size);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Renderable>(ground, shape, color, size, Layer::Middle, sprite);
    world.AddComponentToEntity<Collideable>(ground, boxCollideable, draftBoxCollideable);
    world.AddComponentToEntity<RigidBody>(ground, velocity, 0.f);
    world.AddComponentToEntity<Fatal>(ground);
    world.AddComponentToEntity<Responser>(ground);
    world.AddComponentToEntity<Animation>(ground, keyframes, LoopMode::LoopReverse, 0, true, texureSizeByFrame);
}

void SpawnService::SpawnElement(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color)
{
    Entity element = world.AddEntity();

    sf::Vector2f velocity = sf::Vector2f { 0, 0 };

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);
    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);
    shape->setSize(size);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Renderable>(element, nullptr, color, size, Layer::Middle, nullptr);
    world.AddComponentToEntity<Collideable>(element, boxCollideable, draftBoxCollideable);
    world.AddComponentToEntity<RigidBody>(element, velocity, 0.f);
    world.AddComponentToEntity<Responser>(element);
}

void SpawnService::SpawnLightDrop(World& world, const sf::FloatRect& levelLimits)
{
    Entity lightDrop = world.AddEntity();

    int depth = GetRandomBetween(2, 5);
    int width = 3;
    int length = 20 * depth;
    int x = GetRandomBetween(static_cast<int>(levelLimits.left), static_cast<int>(levelLimits.width));
    int y = 0 - length;
    float speedTweak = 1 + length / 5.0f;
    float speed = 50 * speedTweak;

    sf::Vector2f center = sf::Vector2f { static_cast<float>(x), static_cast<float>(y) };
    sf::Vector2f size = sf::Vector2f { static_cast<float>(width), static_cast<float>(length) };
    sf::Color color = sf::Color { 140, 130, 215, 100 };
    sf::Color outlineColor = sf::Color { 140, 130, 215, 50 };
    sf::Vector2f velocity = sf::Vector2f { 0, speed };

    sf::Transformable transformable;
    transformable.setPosition(center);

    EllipseShape* shape = new EllipseShape { size };
    shape->setOrigin(size * 0.5f);
    shape->setPosition(center);

    shape->setFillColor(color);
    shape->setOutlineThickness(2);
    shape->setOutlineColor(outlineColor);

    // TODO: keep the same transformable object?
    world.AddComponentToEntity<Transformable>(lightDrop, transformable, transformable);
    world.AddComponentToEntity<Renderable>(lightDrop, shape, color, size, Layer::Back, nullptr);
    world.AddComponentToEntity<RigidBody>(lightDrop, velocity, 400.f);
}

void SpawnService::SpawnLightBall(World& world, const sf::FloatRect& levelLimits)
{
    Entity lightBall = world.AddEntity();

    int width = 60;
    int length = 70;
    int x = GetRandomBetween(static_cast<int>(levelLimits.left), static_cast<int>(levelLimits.width));
    int y = static_cast<int>(levelLimits.height) + 400;
    // TODO: speed tweak
    float speedTeak = 1 + GetRandomBetween(1, 5) / 5.f;
    float speed = -200 * speedTeak;

    sf::Vector2f center = sf::Vector2f { static_cast<float>(x), static_cast<float>(y) };
    sf::Vector2f size = sf::Vector2f { static_cast<float>(width), static_cast<float>(length) };
    sf::Color color = sf::Color { 172, 243, 249, 190 };
    sf::Vector2f velocity = sf::Vector2f { 0, speed };

    sf::Transformable transformable;
    transformable.setPosition(center);

    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);
    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    sf::Texture* texture = new sf::Texture {};
    if (!texture->loadFromFile("Assets/lightball_5.png")) {
        std::cout << "Unable to load asset" << std::endl;
    }

    sf::Sprite* sprite = new sf::Sprite {};
    sf::Vector2f spriteOrigin { size.x / 7.f, size.y / 9.f };
    sprite->setTexture(*texture);
    sprite->setOrigin(spriteOrigin);
    sprite->setPosition(center);
    sprite->scale(3.3f, 5.0f);

    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);
    shape->setSize(size);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Transformable>(lightBall, transformable, transformable);
    world.AddComponentToEntity<Renderable>(lightBall, nullptr, color, size, Layer::Middle, sprite);
    world.AddComponentToEntity<RigidBody>(lightBall, velocity, 400.f);
    world.AddComponentToEntity<Grippable>(lightBall);
    world.AddComponentToEntity<Collideable>(lightBall, boxCollideable, draftBoxCollideable);
}
