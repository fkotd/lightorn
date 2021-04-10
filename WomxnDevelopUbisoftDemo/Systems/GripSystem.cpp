#include "stdafx.h"

#include "GripSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/Gripper.hpp"
#include "Components/Transformable.hpp"

void GripSystem::Update(World& world, float deltaTime)
{
    std::set<Entity> grippableEntities = world.Find(GetSignature());

    Signature gripperSignature = *world.GetSignature<Gripper, Transformable, Collideable>();
    std::set<Entity> gripperEntities = world.Find(gripperSignature);

    for (auto grippableEntity : grippableEntities) {
        Collideable& grippableCollideable = world.GetComponent<Collideable>(grippableEntity);

        for (auto gripperEntity : gripperEntities) {
            if (grippableEntity != gripperEntity) {
                Collideable& gripperCollideable = world.GetComponent<Collideable>(gripperEntity);

                if (grippableCollideable.draftBoxCollideable.IsColliding(gripperCollideable.draftBoxCollideable)
                    && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    // make the gripper follow the grippable
                    Transformable& grippableTransformable = world.GetComponent<Transformable>(grippableEntity);
                    Transformable& gripperTransformable = world.GetComponent<Transformable>(gripperEntity);

                    gripperTransformable.draftTransform.setPosition(grippableTransformable.transform.getPosition());
                    gripperCollideable.draftBoxCollideable.SetCenter(grippableCollideable.boxCollideable.GetCenter());
                }
            }
        }
    }
}
