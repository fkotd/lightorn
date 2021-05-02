#include "stdafx.h"

#include "SpawnService.hpp"

#include "Core/Entity.hpp"
#include "Engine/EllipseShape.hpp"
#include "EntityBuilder.hpp"
#include "Tools/Feeling.hpp"
#include "Tools/Layer.hpp"
#include "Tools/LoopMode.hpp"
#include "Tools/Random.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <time.h>

SpawnService::SpawnService()
{
    srand((unsigned)time(0));
}

void SpawnService::SpawnLevel(World& world, const sf::FloatRect& levelLimits)
{
    Entity character = SpawnCharacter(world, levelLimits);
    std::cout << "Character entity id = " << character << std::endl;

    Entity heart = SpawnHeart(world, levelLimits);
    std::cout << "Heart entity id = " << heart << std::endl;

    SpawnLimitElements(world, levelLimits);

    SpawnPlatforms(world, levelLimits);
}

void SpawnService::SpawnLimitElements(World& world, const sf::FloatRect& levelLimits)
{
    Entity ground = SpawnGround(world, levelLimits);
    std::cout << "Ground entity id = " << ground << std::endl;

    float levelLeft = levelLimits.left;
    float levelHeight = levelLimits.height;
    float leftEdgeX = levelLeft;
    float leftEdgeY = levelHeight / 2.f;
    float leftEdgeWidth = 10.f;
    float leftEdgeHeight = levelHeight * 2;

    Entity leftEdge = SpawnEdge(
        world,
        sf::Vector2f { leftEdgeX, leftEdgeY },
        sf::Vector2f { leftEdgeWidth, leftEdgeHeight },
        sf::Color::Cyan,
        false);
    std::cout << "Left edge entity id = " << leftEdge << std::endl;

    float levelRight = levelLimits.left + levelLimits.width;
    float rightEdgeX = levelRight;
    float rightEdgeY = levelHeight / 2.f;
    float rightEdgeWidth = 10.f;
    float rightEdgeHeight = levelHeight * 2;

    Entity rightEdge = SpawnEdge(
        world,
        sf::Vector2f { rightEdgeX, rightEdgeY },
        sf::Vector2f { rightEdgeWidth, rightEdgeHeight },
        sf::Color::Cyan,
        false);
    std::cout << "Right edge entity id = " << rightEdge << std::endl;
}

void SpawnService::SpawnPlateform(World& world, const sf::FloatRect& levelLimits, float yMin, float yMax, float probability)
{
    if (yMax - yMin < 200.f) {
        return;
    }

    // Get a virtual platform, it may be not created
    float platformY = GetRandomFloatBetween(yMin, yMax);
    float plateformHeight = 40.f;

    // Get new y min and max for next elements
    float aboveYMin = yMin;
    float aboveYMax = platformY;
    float belowYMin = platformY + plateformHeight;
    float belowYMax = yMax;

    // Create the element following probability
    float p = GetRandomFloat();
    if (p > probability) {
        std::cout << "Make it!"
                  << "p=" << p << " probablity=" << probability << std::endl;
        float leftLimit = levelLimits.left;
        float rightLimit = levelLimits.left + levelLimits.width;
        float plateformX = GetRandomFloatBetween(leftLimit, rightLimit);

        float platformWidth = 220.f;

        Entity plateform = SpawnPlatform(
            world,
            sf::Vector2f { plateformX, platformY },
            sf::Vector2f { platformWidth, plateformHeight },
            sf::Vector2f { 6.8f, 5.5f },
            sf::Vector2f { 3.5f, 3.5f },
            sf::Color::Cyan,
            false);
        std::cout << "Platform entity id = " << plateform << std::endl;
    } else {
        std::cout << "Don't make it..."
                  << "p=" << p << " probablity=" << probability << std::endl;
    }

    SpawnPlateform(world, levelLimits, aboveYMin, aboveYMax, probability / 2);
    SpawnPlateform(world, levelLimits, belowYMin, belowYMax, probability / 2);
}

void SpawnService::SpawnPlatforms(World& world, const sf::FloatRect& levelLimits)
{
    float yMin = levelLimits.top;
    float yMax = levelLimits.top + levelLimits.height;

    SpawnPlateform(world, levelLimits, yMin, yMax, 0.8);
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
    Feeling feeling = Feeling::Neutral;

    std::map<int, sf::Vector2i> keyframes;
    keyframes[0] = sf::Vector2i(0, 0);
    keyframes[1] = sf::Vector2i(0, 32);
    keyframes[2] = sf::Vector2i(0, 32 * 2);
    keyframes[3] = sf::Vector2i(0, 32 * 3);
    keyframes[4] = sf::Vector2i(0, 32 * 4);
    keyframes[5] = sf::Vector2i(0, 32 * 5);
    keyframes[6] = sf::Vector2i(0, 32 * 6);
    keyframes[7] = sf::Vector2i(0, 32 * 7);
    keyframes[8] = sf::Vector2i(0, 32 * 8);

    sf::Vector2i texureSizeByFrame(32, 32);

    return EntityBuilder(world)
        .AddCameraCenter(world)
        .AddDynamic(world)
        .AddGripper(world)
        .AddMortal(world)
        .AddTransformable(world, center)
        .AddCollideable(world, center, size)
        .AddPhysicBody(world, 400.f, 30.f, 0.90f, 50.f, 10.f)
        .AddRigidBody(world, sf::Vector2f { 0, 0 }, 400.f) // FIXME: duplicate propriety
        .AddRenderable(world, center, size, sf::Color::Magenta, Layer::Top, false)
        .AddSprite(world, center, size, "Assets/eevy_spritesheet.png", spriteOriginFactor, spriteScaleFactor, false)
        .AddAnimation(world, keyframes, LoopMode::Loop, 0, true, texureSizeByFrame)
        .AddFeel(world, feeling)
        .AddObscurity(world)
        .Build();
}

Entity SpawnService::SpawnHeart(World& world, const sf::FloatRect& levelLimits)
{
    float windowWidth = (2 * levelLimits.left) + levelLimits.width;

    float x = windowWidth / 2.f;
    float y = -350.f;
    float width = 45.f;
    float height = 95.f;

    sf::Vector2f center { x, y };
    sf::Vector2f size { width, height };
    sf::Vector2f spriteOriginFactor { 3.f, 6.f };
    sf::Vector2f spriteScaleFactor { 3.f, 3.f };

    std::map<int, sf::Vector2i> keyframes;
    keyframes[0] = sf::Vector2i(0, 0);
    keyframes[1] = sf::Vector2i(0, 32);
    keyframes[2] = sf::Vector2i(0, 32 * 2);
    keyframes[3] = sf::Vector2i(0, 32 * 3);
    keyframes[4] = sf::Vector2i(0, 32 * 4);
    keyframes[5] = sf::Vector2i(0, 32 * 5);
    keyframes[6] = sf::Vector2i(0, 32 * 6);
    keyframes[7] = sf::Vector2i(0, 32 * 7);
    keyframes[8] = sf::Vector2i(0, 32 * 8);

    sf::Vector2i texureSizeByFrame(32, 32);

    return EntityBuilder(world)
        .AddRenderable(world, center, size, sf::Color::Green, Layer::Middle, false)
        .AddRigidBody(world, sf::Vector2f { 0, 0 }, 0)
        .AddCollideable(world, center, size)
        .AddGrippable(world)
        .AddReborner(world)
        .AddSprite(world, center, size, "Assets/heart_spritesheet.png", spriteOriginFactor, spriteScaleFactor, false)
        .AddAnimation(world, keyframes, LoopMode::LoopReverse, 0, true, texureSizeByFrame)
        .Build();
}

Entity SpawnService::SpawnGround(World& world, const sf::FloatRect& levelLimits)
{
    float levelHeight(levelLimits.height);
    float windowWidth = (2 * levelLimits.left) + levelLimits.width;

    float x = windowWidth / 2.f;
    float y = levelHeight;
    float width = windowWidth;
    float height = 3.f;

    sf::Vector2f center { x, y };
    sf::Vector2f size { width, height };
    sf::Vector2f spriteOriginFactor { 20.f, 2.5f };
    sf::Vector2f spriteScaleFactor { 10.f, 10.f };

    std::map<int, sf::Vector2i> keyframes;
    keyframes[0] = sf::Vector2i(0, 0);
    keyframes[1] = sf::Vector2i(0, 64);
    keyframes[2] = sf::Vector2i(0, 2 * 64);

    sf::Vector2i texureSizeByFrame(1000, 64);

    return EntityBuilder(world)
        .AddRenderable(world, center, size, sf::Color::Yellow, Layer::Middle, false)
        .AddSprite(world, center, size, "Assets/ground_spritesheet.png", spriteOriginFactor, spriteScaleFactor, true)
        .AddCollideable(world, center, size)
        .AddFatal(world)
        .AddStatic(world)
        .AddAnimation(world, keyframes, LoopMode::LoopReverse, 0, true, texureSizeByFrame)
        .Build();
}

Entity SpawnService::SpawnPlatform(World& world, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Vector2f& spriteOriginFactor, const sf::Vector2f& spriteScaleFactor, const sf::Color& color, bool isShapeVisible)
{
    return EntityBuilder(world)
        .AddRenderable(world, center, size, color, Layer::Middle, isShapeVisible)
        .AddSprite(world, center, size, "Assets/platform.png", spriteOriginFactor, spriteScaleFactor, true)
        .AddCollideable(world, center, size)
        .AddStatic(world)
        .Build();
}

Entity SpawnService::SpawnEdge(World& world, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Color& color, bool isShapeVisible)
{
    return EntityBuilder(world)
        .AddRenderable(world, center, size, sf::Color::Cyan, Layer::Middle, isShapeVisible)
        .AddCollideable(world, center, size)
        .AddStatic(world)
        .Build();
}

void SpawnService::SpawnLightDrop(World& world, const sf::FloatRect& levelLimits, const sf::Color& color)
{
    int depth = GetRandomIntBetween(2, 5);
    float width = 3.f;
    float length = 20.f * depth;
    float x = GetRandomIntBetween(levelLimits.left, levelLimits.width);
    float y = 0 - length;
    float speedTweak = 1 + length / 5.0f;
    float speed = 50 * speedTweak;

    sf::Vector2f center = sf::Vector2f { x, y };
    sf::Vector2f size = sf::Vector2f { width, length };
    sf::Color outlineColor = sf::Color { 140, 130, 215, 50 };
    sf::Vector2f velocity = sf::Vector2f { 0, speed };

    EntityBuilder(world)
        .AddEllipseRenderable(world, center, size, color, outlineColor, Layer::Back)
        .AddTransformable(world, center)
        .AddRigidBody(world, velocity, 400.f)
        .Build();
}

void SpawnService::SpawnLightBall(World& world, const sf::FloatRect& levelLimits)
{
    float width = 60.f;
    float length = 70.f;
    float x = GetRandomIntBetween(levelLimits.left, levelLimits.width);
    float y = levelLimits.height + 400.f;
    float speedTeak = 1 + GetRandomIntBetween(1, 5) / 5.f;
    float speed = -200 * speedTeak;
    int feelingId = GetRandomIntBetween(0, Feeling::FEELINGS_NUMBER);
    Feeling feeling = static_cast<Feeling>(feelingId);

    std::string spriteName;

    switch (feeling) {
    case Neutral:
        spriteName = "Assets/lightball_neutral.png";
        break;
    case Compassion:
        spriteName = "Assets/lightball_compassion.png";
        break;
    case Fear:
        spriteName = "Assets/lightball_fear.png";
        break;
    case Rage:
        spriteName = "Assets/lightball_rage.png";
        break;
    default:
        std::cout << "Unknow feeling, strange impression..." << std::endl;
        break;
    }

    sf::Vector2f center = sf::Vector2f { x, y };
    sf::Vector2f size = sf::Vector2f { width, length };
    sf::Color color = sf::Color { 172, 243, 249, 190 };
    sf::Vector2f velocity = sf::Vector2f { 0, speed };
    sf::Vector2f spriteOriginFactor { 7.f, 9.f };
    sf::Vector2f spriteScaleFactor { 3.3f, 5.0f };

    EntityBuilder(world)
        .AddRenderable(world, center, size, color, Layer::Middle, false)
        .AddSprite(world, center, size, spriteName, spriteOriginFactor, spriteScaleFactor, true)
        .AddTransformable(world, center)
        .AddRigidBody(world, velocity, 400.f)
        .AddCollideable(world, center, size)
        .AddGrippable(world)
        .AddFeel(world, feeling)
        .Build();
}
