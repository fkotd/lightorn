#pragma once

#include "Core/World.hpp"

class CollisionSystem : public System {
public:
    void Update(World& world);
};
