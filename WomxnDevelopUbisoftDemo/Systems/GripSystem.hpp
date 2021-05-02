#pragma once

#include "Core/Entity.hpp"
#include "Core/System.hpp"
#include "Core/World.hpp"
#include <SFML/Graphics/Rect.hpp>

class GripSystem : public System {
public:
    void Update(World& world, sf::FloatRect levelLimits);

private:
    void UpdateGripperVelocity(World& world, sf::FloatRect levelLimits, Entity gripperEntity, Entity grippableEntity);
    void UpdateGripperFeeling(World& world, Entity gripperEntity, Entity grippableEntity);
    void ResetGripperFeeling(World& world, Entity gripperEntity);
    void ResetGripperVelocity(World& world, Entity gripperEntity);
    const bool IsGrippedToReborner(World& world, Entity entity, Entity otherEntity);
    const void SendGripToRebornerEvent(World& world);
};
