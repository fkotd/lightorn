#include "stdafx.h"

#include "Components/Darkness.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Core/Entity.hpp"
#include "DarknessSystem.hpp"
#include <SFML/System/Vector2.hpp>
#include <set>

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
            darkness.centerRadius = std::max(0.f, darkness.centerRadius - 2.f);
            darkness.middleRadius = std::max(0.f, darkness.middleRadius - 1.5f);
            darkness.outerRadius = std::max(0.f, darkness.outerRadius - 1.f);
        } else {
            darkness.centerRadius = std::min(darkness.centerRadius + 1.f, 300.f);
            darkness.middleRadius = std::min(darkness.middleRadius + 1.5f, 700.f);
            darkness.outerRadius = std::min(darkness.outerRadius + 2.0f, 900.f);
        }

        darkness.shader->setUniform("obscurityColor", sf::Glsl::Vec4(darkness.shape->getFillColor()));
        darkness.shader->setUniform("center", darknessShapePosition);
        darkness.shader->setUniform("centerRadius", darkness.centerRadius);
        darkness.shader->setUniform("middleRadius", darkness.middleRadius);
        darkness.shader->setUniform("outerRadius", darkness.outerRadius);
    }
}
