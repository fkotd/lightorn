#pragma once

#include "System.hpp"
#include "World.hpp"

class CollisionSystem : public System {
public:
    void Update(const std::unique_ptr<World>& world);
};
