#include "stdafx.h"

#include "DestroySystem.hpp"

#include "Components/Transformable.hpp"

void DestroySystem::DestroyOffScreen(World& world, sf::FloatRect levelLimits)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity);

        if (transformable.transform.getPosition().y > levelLimits.height + 800 || transformable.transform.getPosition().y < levelLimits.top - 500) {
            world.RemoveEntity(entity);
        }
    }
}
