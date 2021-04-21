#pragma once

#include "Core/World.hpp"
#include "Tools/Layer.hpp"
#include "Tools/LoopMode.hpp"

class EntityBuilder {
public:
    EntityBuilder(World& world);

    EntityBuilder& AddTransformable(World& world, const sf::Vector2f& center);
    EntityBuilder& AddRenderable(World& world, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Color& color, const Layer layer);
    EntityBuilder& AddSprite(World& world, const sf::Vector2f& center, const sf::Vector2f& size, std::string spritePath, const sf::Vector2f& spriteOriginFactor, const sf::Vector2f& spriteScaleFactor, bool repeat);
    EntityBuilder& AddAnimation(World& world, std::map<int, sf::Vector2i>& keyframes, LoopMode loopMode, int currentKeyframe, bool increase, sf::Vector2i& textureSize);
    EntityBuilder& AddRigidBody(World& world, const sf::Vector2f& veloctity, float maxVelocity);
    EntityBuilder& AddPhysicBody(World& world, float maxSpeed, float speedInc, float slowdownRate, float mass, float gamma);
    EntityBuilder& AddCameraCenter(World& world);
    EntityBuilder& AddCollideable(World& world, const sf::Vector2f& center, const sf::Vector2f& size);
    EntityBuilder& AddGripper(World& world);
    EntityBuilder& AddMortal(World& world);
    EntityBuilder& AddFatal(World& world);
    EntityBuilder& AddReborner(World& world);
    EntityBuilder& AddDynamic(World& world);
    EntityBuilder& AddStatic(World& world);

    Entity Build();

private:
    Entity entity;
};
