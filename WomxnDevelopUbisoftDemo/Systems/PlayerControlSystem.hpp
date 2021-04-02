#pragma once

#include "Core/World.hpp"

class PlayerControlSystem : public System {
public:
    void Update(World& world, float deltaTime);
};
