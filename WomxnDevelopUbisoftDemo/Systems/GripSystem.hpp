#pragma once

#include "Core/World.hpp"

class GripSystem : public System {
public:
    void Update(World& world, sf::FloatRect levelLimits);

private:
    void UpdateFeeling(World& world, Entity gripperEntity, Entity grippableEntity);
    void ResetFeeling(World& world, Entity gripperEntity);
};
