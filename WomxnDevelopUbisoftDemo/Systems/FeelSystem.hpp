#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"
#include "Tools/Feeling.hpp"

class FeelSystem : public System {
public:
    void Update(World& world);

private:
    Feeling GetFeelingChange(World& world);
};
