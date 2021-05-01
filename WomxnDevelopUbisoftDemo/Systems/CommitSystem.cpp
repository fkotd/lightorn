#include "stdafx.h"

#include "CommitSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/Transformable.hpp"
#include "Core/Entity.hpp"
#include <set>

void CommitSystem::Commit(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity);
        transformable.transformable.setPosition(transformable.draftTransformable.getPosition());

        Collideable* collideable = world.GetComponentIfExists<Collideable>(entity);
        if (collideable != nullptr) {
            collideable->boxCollideable.SetCenter(collideable->draftBoxCollideable.GetCenter());
        }

        // Reset draft
        transformable.draftTransformable.setPosition(0, 0);
    }
}
