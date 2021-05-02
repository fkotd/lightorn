#include "stdafx.h"

#include "DestroySystem.hpp"

#include "Components/Transformable.hpp"
#include "Core/Entity.hpp"
#include <set>

static const float BOTTOM_DESTOY_OFFSET = 800.f;
static const float TOP_DESTOY_OFFSET = 500.f;

void DestroySystem::DestroyOffScreen(World& world, const sf::FloatRect& levelLimits)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity);

        if (transformable.transformable.getPosition().y > levelLimits.height + BOTTOM_DESTOY_OFFSET
            || transformable.transformable.getPosition().y < levelLimits.top - TOP_DESTOY_OFFSET) {
            world.RemoveEntity(entity);
        }
    }
}
