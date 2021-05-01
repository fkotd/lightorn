#include "stdafx.h"

#include "CollisionSystem.hpp"

#include "Components/Dynamic.hpp"
#include "Components/Fatal.hpp"
#include "Components/Mortal.hpp"
#include "Components/Reborner.hpp"
#include "Components/Transformable.hpp"
#include "Engine/Collision.h"
#include "Tools/Messages.hpp"
#include <cmath>

void CollisionSystem::Update(World& world, float deltaTime)
{
    std::set<Entity> staticEntities = world.Find(GetSignature());

    Signature dynamicEntitySignature = *world.GetSignature<Collideable, Dynamic>();
    std::set<Entity> dynamicEntities = world.Find(dynamicEntitySignature);

    for (auto dynamicEntity : dynamicEntities) {
        for (auto staticEntity : staticEntities) {
            if (dynamicEntity != staticEntity) {

                Collideable& dynamicCollideable = world.GetComponent<Collideable>(dynamicEntity);
                Collideable& staticCollideable = world.GetComponent<Collideable>(staticEntity);

                if (dynamicCollideable.draftBoxCollideable.IsColliding(staticCollideable.draftBoxCollideable)) {

                    //if (IsFatalCollision(world, dynamicEntity, staticEntity)) {
                    //    world.AddGameEvent(END_GAME_DEATH, Event(true, false));
                    //}

                    if (IsRebornCollision(world, dynamicEntity, staticEntity)) {
                        world.AddGameEvent(END_GAME_REBORN, Event(true, false));
                    }

                    CuteSweeptAABB(world, dynamicEntity, staticEntity, deltaTime);
                }
            }
        }
    }
}

void CollisionSystem::CuteSweeptAABB(World& world, Entity dynamicEntity, Entity staticEntity, float deltaTime)
{
    Collideable& collideable = world.GetComponent<Collideable>(dynamicEntity);
    Collideable& otherCollideable = world.GetComponent<Collideable>(staticEntity);
    RigidBody& rigidBody = world.GetComponent<RigidBody>(dynamicEntity);
    float deltaX = 0;
    float deltaY = 0;
    bool isCollidingX = false;
    bool isCollidingY = false;

    float ratio = 1;

    if (rigidBody.velocity.x != 0) {
        isCollidingX = IsCollidingX(collideable, otherCollideable, rigidBody, deltaTime, deltaX);
    }
    if (rigidBody.velocity.y != 0) {
        isCollidingY = IsCollidingY(collideable, otherCollideable, rigidBody, deltaTime, deltaY);
    }

    if (isCollidingX) {
        ratio = std::min(ratio, deltaX / rigidBody.velocity.x * deltaTime);
    }
    if (isCollidingY) {
        ratio = std::min(ratio, deltaY / rigidBody.velocity.y * deltaTime);
    }

    if (isCollidingX) {
        rigidBody.velocity.x *= ratio;
    }
    if (isCollidingY) {
        rigidBody.velocity.y *= ratio;
    }

    Transformable* transformable = world.GetComponentIfExists<Transformable>(dynamicEntity);
    if (transformable != nullptr) {
        transformable->draftTransformable.setPosition(transformable->transformable.getPosition() + sf::Vector2f(rigidBody.velocity.x * deltaTime, rigidBody.velocity.y * deltaTime));
    }
}

bool CollisionSystem::IsCollidingX(Collideable& dynamicCollideable, Collideable& staticCollideable, RigidBody& dynamicRigidBody, float deltaTime, float& deltaX)
{
    float dynamicCoordX(0);
    float staticCoordX(0);

    sf::FloatRect boundingBox = dynamicCollideable.draftBoxCollideable.GetBoundingBox();
    sf::FloatRect otherBoundingBox = staticCollideable.draftBoxCollideable.GetBoundingBox();

    float left = boundingBox.left;
    float right = boundingBox.left + boundingBox.width;

    if (dynamicRigidBody.velocity.x > 0) {
        dynamicCoordX = std::max(left, right);
    } else if (dynamicRigidBody.velocity.x < 0) {
        dynamicCoordX = std::min(left, right);
    }

    left = otherBoundingBox.left;
    right = otherBoundingBox.left + otherBoundingBox.width;

    if (dynamicRigidBody.velocity.x > 0) {
        staticCoordX = std::min(left, right);
    } else if (dynamicRigidBody.velocity.x < 0) {
        staticCoordX = std::max(left, right);
    }

    deltaX = staticCoordX - dynamicCoordX;
    float deltaX2 = deltaX + dynamicRigidBody.velocity.x * deltaTime;
    bool isColliding = deltaX * deltaX2 < 0;

    return isColliding;
}

bool CollisionSystem::IsCollidingY(Collideable& dynamicCollideable, Collideable& staticCollideable, RigidBody& dynamicRigidBody, float deltaTime, float& deltaY)
{
    float dynamicCoordY(0);
    float staticCoordY(0);

    sf::FloatRect boundingBox = dynamicCollideable.draftBoxCollideable.GetBoundingBox();
    sf::FloatRect otherBoundingBox = staticCollideable.draftBoxCollideable.GetBoundingBox();

    float top = boundingBox.top;
    float bottom = boundingBox.top + boundingBox.height;

    if (dynamicRigidBody.velocity.y > 0) {
        dynamicCoordY = std::max(top, bottom);
    } else if (dynamicRigidBody.velocity.y < 0) {
        dynamicCoordY = std::min(top, bottom);
    }

    top = otherBoundingBox.top;
    bottom = otherBoundingBox.top + otherBoundingBox.height;

    if (dynamicRigidBody.velocity.y > 0) {
        staticCoordY = std::min(top, bottom);
    } else if (dynamicRigidBody.velocity.y < 0) {
        staticCoordY = std::max(top, bottom);
    }

    deltaY = staticCoordY - dynamicCoordY;
    float deltaY2 = deltaY + dynamicRigidBody.velocity.y * deltaTime;
    bool isColliding = deltaY * deltaY2 < 0;

    return isColliding;
}

const bool CollisionSystem::IsFatalCollision(World& world, Entity entity, Entity otherEntity)
{
    Fatal* fatal = world.GetComponentIfExists<Fatal>(entity);
    Mortal* mortal = world.GetComponentIfExists<Mortal>(entity);

    Fatal* otherFatal = world.GetComponentIfExists<Fatal>(otherEntity);
    Mortal* otherMortal = world.GetComponentIfExists<Mortal>(otherEntity);

    if (fatal != nullptr && otherMortal != nullptr) {
        return true;
    }

    if (mortal != nullptr && otherFatal != nullptr) {
        return true;
    }

    return false;
}

const bool CollisionSystem::IsRebornCollision(World& world, Entity entity, Entity otherEntity)
{
    Reborner* reborner = world.GetComponentIfExists<Reborner>(entity);
    Mortal* mortal = world.GetComponentIfExists<Mortal>(entity);

    Reborner* otherReborner = world.GetComponentIfExists<Reborner>(otherEntity);
    Mortal* otherMortal = world.GetComponentIfExists<Mortal>(otherEntity);

    if (reborner != nullptr && otherMortal != nullptr) {
        return true;
    }

    if (mortal != nullptr && otherReborner != nullptr) {
        return true;
    }

    return false;
}
