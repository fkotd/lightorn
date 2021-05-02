#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"
#include <string>

class DarknessSystem : public System {
public:
    void Update(World& world);

private:
    const bool IsDarknessLevelFatal(World& world, float darknessOuterRadius);
    const bool IsDarknessLevelReborn(World& world, float darknessOuterRadius);
    const bool IsGrippedToReborner(World& world);
    const void SendDarknessEvent(World& world, std::string message);
};
