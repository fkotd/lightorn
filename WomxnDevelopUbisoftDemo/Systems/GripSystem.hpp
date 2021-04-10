#pragma once

#include "Core/World.hpp"

class GripSystem : public System {
public:
    void Update(World& world, float deltaTime);
};
