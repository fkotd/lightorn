#pragma once

#include "Entity.hpp"
#include <unordered_map>

class EventManager {
public:
    void AddCollisionEvent(Entity entity, Entity otherEntity)
    {
        collidedEntities.emplace(entity, otherEntity);
    }

    void ClearEvents()
    {
        collidedEntities.clear();
    }

private:
    std::unordered_map<Entity, Entity> collidedEntities;
};
