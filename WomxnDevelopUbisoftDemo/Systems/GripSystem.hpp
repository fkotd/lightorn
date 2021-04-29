#pragma once

#include "Core/World.hpp"

class GripSystem : public System {
public:
    void Update(World& world, sf::FloatRect levelLimits);

private:
    void UpdateGripperVelocity(World& world, sf::FloatRect levelLimits, Entity gripperEntity, Entity grippableEntity);
    void UpdateGripperFeeling(World& world, Entity gripperEntity, Entity grippableEntity);
    void ResetGripperFeeling(World& world, Entity gripperEntity);
    void ResetGripperVelocity(World& world, Entity gripperEntity);
};
