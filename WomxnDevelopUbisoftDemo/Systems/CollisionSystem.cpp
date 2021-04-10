#include "stdafx.h"

#include "CollisionSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Engine/Collision.h"
#include <cmath>

void CollisionSystem::Update(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity1 : entities) {
        Collideable& collideable1 = world.GetComponent<Collideable>(entity1);

        // TODO: loop in way that entities are not checked twice
        for (auto entity2 : entities) {
            if (entity1 != entity2) {
                Collideable& collideable2 = world.GetComponent<Collideable>(entity2);

                if (collideable1.draftBoxCollideable.IsColliding(collideable2.draftBoxCollideable)) {

                    RigidBody& rigidBody1 = world.GetComponent<RigidBody>(entity1);
                    RigidBody& rigidBody2 = world.GetComponent<RigidBody>(entity2);

                    sf::Vector2f normal;
                    float collisionTime = SweptAABB(collideable1.draftBoxCollideable.GetBoundingBox(), collideable2.draftBoxCollideable.GetBoundingBox(), rigidBody1.velocity, rigidBody2.velocity, normal);
                    float remainingTime = 1.0f - collisionTime;

                    float magnitude = sqrt((rigidBody1.velocity.x * rigidBody1.velocity.x + rigidBody1.velocity.y * rigidBody1.velocity.y)) * remainingTime;
                    float dotProduct = rigidBody1.velocity.x * normal.y + rigidBody1.velocity.y * normal.x;

                    if (dotProduct > 0.0f) {
                        dotProduct = 1.0f;
                    } else if (dotProduct < 0.0f) {
                        dotProduct = -1.0f;
                    }

                    rigidBody1.velocity.x = dotProduct * normal.y * magnitude;
                    rigidBody1.velocity.y = dotProduct * normal.x * magnitude;

                    sf::Vector2f collisionOffset { rigidBody1.velocity.x * collisionTime, rigidBody1.velocity.y * collisionTime };

                    collideable1.draftBoxCollideable.SetCenter(collideable1.boxCollideable.GetCenter() + collisionOffset);

                    Transformable* transformable1 = world.GetComponentIfExists<Transformable>(entity1);
                    if (transformable1 != nullptr) {
                        transformable1->draftTransform.setPosition(transformable1->transform.getPosition() + collisionOffset);
                    }
                }
            }
        }
    }
}

// return the offset of time when the collision occured during the frame
// TODO: add comments
float CollisionSystem::SweptAABB(const sf::FloatRect& r1, const sf::FloatRect& r2, sf::Vector2f& v1, sf::Vector2f& v2, sf::Vector2f& normal)
{
    sf::Vector2f inverseEntry;
    sf::Vector2f inverseExit;

    if (v1.x > 0.0f) {
        inverseEntry.x = r2.left - (r1.left + r1.width);
        inverseExit.x = (r2.left + r2.width) - r1.left;
    } else {
        inverseEntry.x = (r2.left + r2.width) - r1.left;
        inverseExit.x = r2.left - (r1.left + r1.width);
    }

    if (v1.y > 0.0f) {
        inverseEntry.y = r2.top - (r1.top + r1.height);
        inverseExit.y = (r2.top + r2.height) - r1.top;
    } else {
        inverseEntry.y = (r2.top + r2.height) - r1.top;
        inverseExit.y = r2.top - (r1.top + r1.height);
    }

    sf::Vector2f entry;
    sf::Vector2f exit;

    if (v1.x == 0.0f) {
        entry.x = -std::numeric_limits<float>::infinity();
        exit.x = std::numeric_limits<float>::infinity();
    } else {
        entry.x = inverseEntry.x / v1.x;
        exit.x = inverseExit.x / v1.x;
    }

    if (v1.y == 0.0f) {
        entry.y = -std::numeric_limits<float>::infinity();
        exit.y = std::numeric_limits<float>::infinity();
    } else {
        entry.y = inverseEntry.y / v1.y;
        exit.y = inverseExit.y / v1.y;
    }

    float entryTime = std::max(entry.x, entry.y);
    float exitTime = std::min(exit.x, exit.y);

    if (entryTime > exitTime || entry.x < 0.0f && entry.y < 0.0f || entry.x > 1.0f || entry.y > 1.0f) {
        normal.x = 0.0f;
        normal.y = 0.0f;
        return 1.0f;
    } else {
        if (entry.x > entry.y) {
            if (inverseEntry.x < 0.0f) {
                normal.x = 1.0f;
                normal.y = 0.0f;
            } else {
                normal.x = -1.0f;
                normal.y = 0.0f;
            }
        } else {
            if (inverseEntry.y < 0.0f) {
                normal.x = 0.0f;
                normal.y = 1.0f;
            } else {
                normal.x = 0.0f;
                normal.y = -1.0f;
            }
        }
    }
    return entryTime;
}