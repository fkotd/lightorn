#include "stdafx.h"

#include "Components/Darkness.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Core/Entity.hpp"
#include "DarknessSystem.hpp"
#include "Tools/Messages.hpp"
#include <SFML/System/Vector2.hpp>
#include <set>

const float MIN_CENTER_RADIUS = 0;
const float MIN_MIDDLE_RADIUS = 0;
const float MIN_OUTER_RADIUS = 0;

const float MAX_CENTER_RADIUS = 300.f;
const float MAX_MIDDLE_RADIUS = 700.f;
const float MAX_OUTER_RADIUS = 900.f;

void DarknessSystem::Update(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Darkness& darkness = world.GetComponent<Darkness>(entity);
        Transformable& transformable = world.GetComponent<Transformable>(entity);
        RigidBody& rigidBody = world.GetComponent<RigidBody>(entity);

        darkness.shape->setPosition(transformable.transformable.getPosition());
        sf::Vector2f darknessShapePosition = darkness.shape->getPosition();

        if (abs(rigidBody.velocity.y) <= 2.f) {
            darkness.centerRadius = std::max(MIN_CENTER_RADIUS, darkness.centerRadius - 5.f);
            darkness.middleRadius = std::max(MIN_MIDDLE_RADIUS, darkness.middleRadius - 4.f);
            darkness.outerRadius = std::max(MIN_OUTER_RADIUS, darkness.outerRadius - 3.f);
        } else {
            darkness.centerRadius = std::min(darkness.centerRadius + 1.f, MAX_CENTER_RADIUS);
            darkness.middleRadius = std::min(darkness.middleRadius + 1.5f, MAX_MIDDLE_RADIUS);
            darkness.outerRadius = std::min(darkness.outerRadius + 2.0f, MAX_OUTER_RADIUS);
        }

        darkness.shader->setUniform("obscurityColor", sf::Glsl::Vec4(darkness.shape->getFillColor()));
        darkness.shader->setUniform("center", darknessShapePosition);
        darkness.shader->setUniform("centerRadius", darkness.centerRadius);
        darkness.shader->setUniform("middleRadius", darkness.middleRadius);
        darkness.shader->setUniform("outerRadius", darkness.outerRadius);

        if (IsDarknessLevelFatal(darkness.outerRadius)) {
            SendFatalDarknessEvent(world);
        }
    }
}

bool DarknessSystem::IsDarknessLevelFatal(float darknessOuterRadius)
{
    return darknessOuterRadius <= MIN_OUTER_RADIUS;
}

void DarknessSystem::SendFatalDarknessEvent(World& world)
{
    world.AddGameEvent(END_GAME_DEATH, Event(true, false));
}
