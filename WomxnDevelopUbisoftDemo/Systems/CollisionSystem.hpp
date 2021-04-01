#pragma once

#include "Core/World.hpp"

class CollisionSystem : public System {
public:
    void Update(const std::unique_ptr<World>& world);
};
