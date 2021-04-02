#pragma once

#include "Core/World.hpp"

class TransformSystem : public System {
public:
    void Update(World& world, float deltaTime);
};
