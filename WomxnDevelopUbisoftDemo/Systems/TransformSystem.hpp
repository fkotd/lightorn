#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"

class TransformSystem : public System {
public:
    void Update(World& world, float deltaTime);
};
