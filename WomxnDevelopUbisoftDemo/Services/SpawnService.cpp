#include "stdafx.h"

#include "SpawnService.hpp"

#include "Components/Animation.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Dynamic.hpp"
#include "Components/Fatal.hpp"
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
#include "Engine/EllipseShape.hpp"
#include "EntityBuilder.hpp"
#include "Tools/Layer.hpp"
#include "Tools/LoopMode.hpp"
#include "Tools/Random.hpp"

void SpawnService::SpawnLevel(World& world, const sf::FloatRect& levelLimits)
{
    Entity character = SpawnCharacter(world, levelLimits);
    std::cout << "Character entity id = " << character << std::endl;

    Entity heart = SpawnHeart(world, levelLimits);
    std::cout << "Heart entity id = " << heart << std::endl;

    SpawnLimitElements(world, levelLimits);

    SpawnLevelElements(world, levelLimits);
}

void SpawnService::SpawnLimitElements(World& world, const sf::FloatRect& levelLimits)
{
    Entity ground = SpawnGround(world, levelLimits);
    std::cout << "Ground entity id = " << ground << std::endl;

    float levelLeft(levelLimits.left);
    float levelHeight(levelLimits.height);
    float leftEdgeX = levelLeft;
    float leftEdgeY = levelHeight / 2.f;
    float leftEdgeWidth = 10.f;
    float leftEdgeHeight = levelHeight - 20.f;
    Entity leftEdge = SpawnElement( // Left Edge
        world,
        sf::Vector2f { leftEdgeX, leftEdgeY },
        sf::Vector2f { leftEdgeWidth, leftEdgeHeight },
        sf::Color::Cyan);
    std::cout << "Left edge entity id = " << leftEdge << std::endl;

    float levelRight(levelLimits.left + levelLimits.width);
    float rightEdgeX = levelRight;
    float rightEdgeY = levelHeight / 2.f;
    float rightEdgeWidth = 10.f;
    float rightEdgeHeight = levelHeight - 20.f;
    Entity rightEdge = SpawnElement(
        world,
        sf::Vector2f { rightEdgeX, rightEdgeY },
        sf::Vector2f { rightEdgeWidth, rightEdgeHeight },
        sf::Color::Cyan);
    std::cout << "Right edge entity id = " << rightEdge << std::endl;
}

void SpawnService::SpawnLevelElements(World& world, const sf::FloatRect& levelLimits)
{
    float windowWidth = (2 * levelLimits.left) + levelLimits.width;

    float platformX = windowWidth / 2.f;
    float platformY = 700.f;
    float platformWidth = 200.f;
    float platformHeight = 50.f;
    Entity platform = SpawnElement(
        world,
        sf::Vector2f { platformX, platformY },
        sf::Vector2f { platformWidth, platformHeight },
        sf::Color::Cyan);
    std::cout << "Platform entity id = " << platform << std::endl;
}

Entity SpawnService::SpawnCharacter(World& world, const sf::FloatRect& levelLimits)
{
    float windowWidth = (2 * levelLimits.left) + levelLimits.width;

    float x = windowWidth / 2.f;
    float y = 100.f;
    float width = 50.f;
    float height = 50.f;

    sf::Vector2f center { x, y };
    sf::Vector2f size { width, height };
    sf::Vector2f spriteOriginFactor { 3.1f, 2.2f };
    sf::Vector2f spriteScaleFactor { 2.9f, 2.9f };

    return EntityBuilder(world)
        .AddCameraCenter(world)
        .AddDynamic(world)
        .AddGripper(world)
        .AddMortal(world)
        .AddTransformable(world, center)
        .AddCollideable(world, center, size)
        .AddPhysicBody(world, 400.f, 30.f, 0.90f, 50.f, 10.f)
        .AddRigidBody(world, sf::Vector2f { 0, 0 }, 400.f) // FIXME: duplicate propriety
        .AddRenderable(world, center, size, sf::Color::Magenta, Layer::Top)
        .AddSprite(world, center, size, "Assets/eevy.png", spriteOriginFactor, spriteScaleFactor, false)
        .Build();
}

Entity SpawnService::SpawnHeart(World& world, const sf::FloatRect& levelLimits)
{
    float windowWidth = (2 * levelLimits.left) + levelLimits.width;

    float x = windowWidth / 2.f;
    float y = -100.f;
    float width = 100.f;
    float height = 100.f;

    sf::Vector2f center { x, y };
    sf::Vector2f size { width, height };

    return EntityBuilder(world)
        .AddRenderable(world, center, size, sf::Color::Magenta, Layer::Middle)
        .AddRigidBody(world, sf::Vector2f { 0, 0 }, 0)
        .AddCollideable(world, center, size)
        .AddStatic(world)
        .AddReborner(world)
        .Build();
}

Entity SpawnService::SpawnGround(World& world, const sf::FloatRect& levelLimits)
{
    float levelHeight(levelLimits.height);
    float windowWidth = (2 * levelLimits.left) + levelLimits.width;

    float x = windowWidth / 2.f;
    float y = levelHeight;
    float width = windowWidth;
    float height = 5.f; // TODO: change value

    sf::Vector2f center { x, y };
    sf::Vector2f size { width, height };
    sf::Vector2f spriteOriginFactor { 20.f, 2.5f };
    sf::Vector2f spriteScaleFactor { 10.f, 10.f };

    std::map<int, sf::Vector2i> keyframes;
    keyframes[0] = sf::Vector2i(0, 0);
    keyframes[1] = sf::Vector2i(0, 32);
    keyframes[2] = sf::Vector2i(0, 64);

    sf::Vector2i texureSizeByFrame(1000, 32);

    return EntityBuilder(world)
        .AddRenderable(world, center, size, sf::Color::Yellow, Layer::Middle)
        .AddSprite(world, center, size, "Assets/ground_spritesheet.png", spriteOriginFactor, spriteScaleFactor, true)
        .AddCollideable(world, center, size)
        .AddFatal(world)
        .AddStatic(world)
        .AddAnimation(world, keyframes, LoopMode::LoopReverse, 0, true, texureSizeByFrame)
        .Build();
}

Entity SpawnService::SpawnElement(World& world, const sf::Vector2f center, const sf::Vector2f size, const sf::Color color)
{
    return EntityBuilder(world)
        .AddRenderable(world, center, size, sf::Color::Cyan, Layer::Middle)
        .AddCollideable(world, center, size)
        .AddStatic(world)
        .Build();
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
    world.AddComponentToEntity<Renderable>(lightDrop, shape, color, size, Layer::Back);
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
    world.AddComponentToEntity<Renderable>(lightBall, nullptr, color, size, Layer::Middle);
    world.AddComponentToEntity<Sprite>(lightBall, sprite);
    world.AddComponentToEntity<RigidBody>(lightBall, velocity, 400.f);
    world.AddComponentToEntity<Grippable>(lightBall);
    world.AddComponentToEntity<Collideable>(lightBall, boxCollideable, draftBoxCollideable);
}
