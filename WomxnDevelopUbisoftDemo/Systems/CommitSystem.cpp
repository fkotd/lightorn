#include "stdafx.h"

#include "CommitSystem.hpp"
#include "Components/Collideable.hpp"
#include "Components/Transformable.hpp"

void CommitSystem::Commit(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Transformable& transformable = world.GetComponent<Transformable>(entity);
        transformable.transform.setPosition(transformable.draftTransform.getPosition());

        Collideable* collideable = world.GetComponentIfExists<Collideable>(entity);
        if (collideable != nullptr) {
            collideable->boxCollideable.SetCenter(collideable->draftBoxCollideable.GetCenter());
        }
    }
}
