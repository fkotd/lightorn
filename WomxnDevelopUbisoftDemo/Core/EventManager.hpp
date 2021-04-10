#pragma once

#include "Entity.hpp"
#include <unordered_map>

class EventManager {
private:
    std::unordered_map<Entity, Entity> collidedEntities;
};
