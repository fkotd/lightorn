#pragma once

#include "Core/World.hpp"

class PlayerControlSystem : public System {
public:
    void Update(const std::unique_ptr<World>& world, float deltaTime);
};
