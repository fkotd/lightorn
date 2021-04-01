#pragma once

#include "World.hpp"

class TransformSystem : public System {
public:
    void Update(const std::unique_ptr<World>& world, float deltaTime);
};
