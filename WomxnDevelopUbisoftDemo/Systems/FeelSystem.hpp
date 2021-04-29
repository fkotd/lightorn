#pragma once

#include "Core/World.hpp"
#include "Tools/Feeling.hpp"

class FeelSystem : public System {
public:
    void Update(World& world);

private:
    Feeling GetFeelingChange(World& world);
};
