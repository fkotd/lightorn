#include "stdafx.h"

#include "EntityBuilder.hpp"

#include "Components/Animation.hpp"
#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Dynamic.hpp"
#include "Components/Fatal.hpp"
#include "Components/Gripper.hpp"
#include "Components/Mortal.hpp"
#include "Components/PhysicBody.hpp"
#include "Components/Reborner.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Sprite.hpp"
#include "Components/Static.hpp"
#include "Components/Transformable.hpp"

EntityBuilder::EntityBuilder(World& world)
{
    entity = world.AddEntity();
}

EntityBuilder& EntityBuilder::AddRenderable(World& world, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Color& color, const Layer layer)
{
    // TODO: how to delete the new
    sf::RectangleShape* shape = new sf::RectangleShape();
    shape->setPosition(center);
    shape->setOrigin(size * 0.5f);
    shape->setSize(size);

    shape->setFillColor(color);
    shape->setOutlineThickness(1);
    shape->setOutlineColor(color);

    world.AddComponentToEntity<Renderable>(entity, shape, color, size, layer);

    return *this;
}

EntityBuilder& EntityBuilder::AddSprite(World& world, const sf::Vector2f& center, const sf::Vector2f& size, std::string spritePath, const sf::Vector2f& spriteOriginFactor, const sf::Vector2f& spriteScaleFactor, bool repeat)
{
    sf::Texture* texture = new sf::Texture {};
    if (!texture->loadFromFile(spritePath)) {
        std::cout << "Unable to load asset" << std::endl;
    }
    texture->setRepeated(repeat);

    sf::Sprite* sprite = new sf::Sprite {};
    sf::Vector2f spriteOrigin { size.x / spriteOriginFactor.x, size.y / spriteOriginFactor.y };
    sprite->setTexture(*texture);
    sprite->setPosition(center);
    sprite->setOrigin(spriteOrigin);
    sprite->scale(spriteScaleFactor);

    world.AddComponentToEntity<Sprite>(entity, sprite);

    return *this;
}

EntityBuilder& EntityBuilder::AddTransformable(World& world, const sf::Vector2f& center)
{
    // TODO: est ce qu'on peut utiliser le même objet ?
    sf::Transformable transform;
    transform.setPosition(center);

    sf::Transformable draftTransform;
    draftTransform.setPosition(center);

    world.AddComponentToEntity<Transformable>(entity, transform, draftTransform);

    return *this;
}

EntityBuilder& EntityBuilder::AddAnimation(World& world, std::map<int, sf::Vector2i>& keyframes, LoopMode loopMode, int currentKeyframe, bool increase, sf::Vector2i& textureSize)
{
    world.AddComponentToEntity<Animation>(entity, keyframes, loopMode, currentKeyframe, increase, textureSize);

    return *this;
}

EntityBuilder& EntityBuilder::AddCollideable(World& world, const sf::Vector2f& center, const sf::Vector2f& size)
{
    BoxCollideable boxCollideable = BoxCollideable {};
    boxCollideable.SetBoundingBox(center, size);

    BoxCollideable draftBoxCollideable = BoxCollideable {};
    draftBoxCollideable.SetBoundingBox(center, size);

    world.AddComponentToEntity<Collideable>(entity, boxCollideable, draftBoxCollideable);

    return *this;
}

EntityBuilder& EntityBuilder::AddRigidBody(World& world, const sf::Vector2f& velocity, float maxVelocity)
{
    world.AddComponentToEntity<RigidBody>(entity, velocity, maxVelocity);

    return *this;
}

EntityBuilder& EntityBuilder::AddPhysicBody(World& world, float maxSpeed, float speedInc, float slowdownRate, float mass, float gamma)
{
    world.AddComponentToEntity<PhysicBody>(entity, maxSpeed, speedInc, slowdownRate, mass, gamma);

    return *this;
}

EntityBuilder& EntityBuilder::AddCameraCenter(World& world)
{
    world.AddComponentToEntity<CameraCenter>(entity);

    return *this;
}

EntityBuilder& EntityBuilder::AddGripper(World& world)
{
    world.AddComponentToEntity<Gripper>(entity);

    return *this;
}

EntityBuilder& EntityBuilder::AddMortal(World& world)
{
    world.AddComponentToEntity<Mortal>(entity);

    return *this;
}

EntityBuilder& EntityBuilder::AddFatal(World& world)
{
    world.AddComponentToEntity<Fatal>(entity);

    return *this;
}

EntityBuilder& EntityBuilder::AddReborner(World& world)
{
    world.AddComponentToEntity<Reborner>(entity);

    return *this;
}

EntityBuilder& EntityBuilder::AddDynamic(World& world)
{
    world.AddComponentToEntity<Dynamic>(entity);

    return *this;
}

EntityBuilder& EntityBuilder::AddStatic(World& world)
{
    world.AddComponentToEntity<Static>(entity);

    return *this;
}

Entity EntityBuilder::Build()
{
    return entity;
}
