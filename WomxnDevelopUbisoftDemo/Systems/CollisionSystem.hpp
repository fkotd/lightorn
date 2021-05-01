#pragma once

#include "Core/World.hpp"

#include "Components/Collideable.hpp"
#include "Components/RigidBody.hpp"
#include "Core/Entity.hpp"

class CollisionSystem : public System {
public:
    void Update(World& world, float deltaTime);

private:
    void CuteSweeptAABB(World& world, Entity dynamicEntity, Entity staticEntity, float deltaTime);
    bool IsCollidingX(Collideable& dynamicCollideable, Collideable& staticCollideable, RigidBody& dynamicRigidBody, float deltaTime, float& deltaX);
    bool IsCollidingY(Collideable& dynamicCollideable, Collideable& staticCollideable, RigidBody& dynamicRigidBody, float deltaTime, float& deltaY);
    const bool IsFatalCollision(World& world, Entity entity, Entity otherEntity);
    const bool IsRebornCollision(World& world, Entity entity, Entity otherEntity);
};
