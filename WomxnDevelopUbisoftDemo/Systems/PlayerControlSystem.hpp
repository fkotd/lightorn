#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"

class PlayerControlSystem : public System {
public:
    void Update(World& world);
};
