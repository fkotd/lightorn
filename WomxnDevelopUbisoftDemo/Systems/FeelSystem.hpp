#pragma once

#include "Core/World.hpp"
#include "Tools/Feeling.hpp"

class FeelSystem : public System {
public:
    void Update(World& world, float deltaTime);

private:
    Feeling GetFeelingChange(World& world);
};
