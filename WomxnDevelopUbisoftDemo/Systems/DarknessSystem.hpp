#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"

class DarknessSystem : public System {
public:
    void Update(World& world);

private:
    bool IsDarknessLevelFatal(float darknessOuterRadius);
    void SendFatalDarknessEvent(World& world);
};
