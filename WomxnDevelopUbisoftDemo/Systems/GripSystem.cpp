#include "stdafx.h"

#include "GripSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/Feel.hpp"
#include "Components/Gripper.hpp"
#include "Components/Mortal.hpp"
#include "Components/Reborner.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transformable.hpp"
#include "Core/Event.hpp"
#include "Tools/Feeling.hpp"
#include "Tools/Messages.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <set>

static const float Y_VELOCITY_AFTER_GRIP = 200.f;

void GripSystem::Update(World& world, sf::FloatRect levelLimits)
{
    std::set<Entity> grippableEntities = world.Find(GetSignature());

    Signature gripperSignature = *world.GetSignature<Gripper, Transformable, Collideable, RigidBody>();
    std::set<Entity> gripperEntities = world.Find(gripperSignature);

    for (auto gripperEntity : gripperEntities) {
        Collideable& gripperCollideable = world.GetComponent<Collideable>(gripperEntity);

        bool hasCollided = false;

        for (auto grippableEntity : grippableEntities) {
            Collideable& grippableCollideable = world.GetComponent<Collideable>(grippableEntity);

            if (grippableEntity != gripperEntity) {

                if (grippableCollideable.draftBoxCollideable.IsColliding(gripperCollideable.draftBoxCollideable)
                    && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

                    hasCollided = true;

                    if (IsGrippedToReborner(world, gripperEntity, grippableEntity)) {
                        SendGripToRebornerEvent(world);
                    }

                    UpdateGripperFeeling(world, gripperEntity, grippableEntity);
                    UpdateGripperVelocity(world, levelLimits, gripperEntity, grippableEntity);
                }
            }
        }

        if (!hasCollided) {
            world.RemoveGameEvent(GRIP_FEELING);
            ResetGripperFeeling(world, gripperEntity);
            ResetGripperVelocity(world, gripperEntity);
        }
    }
}

void GripSystem::UpdateGripperVelocity(World& world, sf::FloatRect levelLimits, Entity gripperEntity, Entity grippableEntity)
{
    Transformable& gripperTransformable = world.GetComponent<Transformable>(gripperEntity);
    RigidBody& grippableRigidBody = world.GetComponent<RigidBody>(grippableEntity);
    RigidBody& gripperRigidBody = world.GetComponent<RigidBody>(gripperEntity);

    gripperRigidBody.velocity = grippableRigidBody.velocity;

    // TODO: set better top level limit because here it is a bit non sense
    // Avoid the character entity to be removed
    if (gripperTransformable.transformable.getPosition().y < levelLimits.top - 450) {
        ResetGripperVelocity(world, gripperEntity);
    }
}

void GripSystem::UpdateGripperFeeling(World& world, Entity gripperEntity, Entity grippableEntity)
{
    Feel* gripperFeel = world.GetComponentIfExists<Feel>(gripperEntity);
    Feel* grippableFeel = world.GetComponentIfExists<Feel>(grippableEntity);

    if (gripperFeel != nullptr && grippableFeel != nullptr) {
        if (gripperFeel->feeling != grippableFeel->feeling) {
            gripperFeel->feeling = grippableFeel->feeling;

            Feeling gripFeeling(gripperFeel->feeling);

            world.AddGameEvent(GRIP_FEELING, Event(gripFeeling, true));
        }

        ImGui::Begin("Feeling Infos");
        ImGui::Text("Lightball feeling: %d", grippableFeel->feeling);
        ImGui::Text("Character feeling: %d", gripperFeel->feeling);
        ImGui::End();
    }
}

void GripSystem::ResetGripperFeeling(World& world, Entity gripperEntity)
{
    Feel* gripperFeel = world.GetComponentIfExists<Feel>(gripperEntity);
    gripperFeel->feeling = Feeling::Neutral;
}

void GripSystem::ResetGripperVelocity(World& world, Entity gripperEntity)
{
    RigidBody& gripperRigidBody = world.GetComponent<RigidBody>(gripperEntity);
    if (gripperRigidBody.velocity.y < 0) {
        gripperRigidBody.velocity.y = Y_VELOCITY_AFTER_GRIP;
    }
}

const bool GripSystem::IsGrippedToReborner(World& world, Entity entity, Entity otherEntity)
{
    Reborner* reborner = world.GetComponentIfExists<Reborner>(entity);
    Mortal* mortal = world.GetComponentIfExists<Mortal>(entity);

    Reborner* otherReborner = world.GetComponentIfExists<Reborner>(otherEntity);
    Mortal* otherMortal = world.GetComponentIfExists<Mortal>(otherEntity);

    if (reborner != nullptr && otherMortal != nullptr) {
        return true;
    }

    if (mortal != nullptr && otherReborner != nullptr) {
        return true;
    }

    return false;
}

const void GripSystem::SendGripToRebornerEvent(World& world)
{
    world.AddGameEvent(GRIP_TO_REBORNER, Event(true, false));
}
