#include "stdafx.h"

#include "GripSystem.hpp"

#include "Components/Collideable.hpp"
#include "Components/Feel.hpp"
#include "Components/Gripper.hpp"
#include "Components/Transformable.hpp"
#include "Tools/Messages.hpp"

void GripSystem::Update(World& world, sf::FloatRect levelLimits)
{
    std::set<Entity> grippableEntities = world.Find(GetSignature());

    Signature gripperSignature = *world.GetSignature<Gripper, Transformable, Collideable>();
    std::set<Entity> gripperEntities = world.Find(gripperSignature);

    bool hasCollided = false;

    for (auto gripperEntity : gripperEntities) {
        Collideable& gripperCollideable = world.GetComponent<Collideable>(gripperEntity);

        for (auto grippableEntity : grippableEntities) {
            Collideable& grippableCollideable = world.GetComponent<Collideable>(grippableEntity);

            if (grippableEntity != gripperEntity) {

                if (grippableCollideable.draftBoxCollideable.IsColliding(gripperCollideable.draftBoxCollideable)
                    && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

                    hasCollided = true;

                    UpdateFeeling(world, gripperEntity, grippableEntity);

                    // make the gripper follow the grippable
                    Transformable& grippableTransformable = world.GetComponent<Transformable>(grippableEntity);
                    Transformable& gripperTransformable = world.GetComponent<Transformable>(gripperEntity);

                    sf::Vector2f grippablePosition = grippableTransformable.transform.getPosition();
                    sf::Vector2f gripPosition;
                    gripPosition.x = std::max(levelLimits.left, grippablePosition.x);
                    gripPosition.x = std::min(levelLimits.left + levelLimits.width, gripPosition.x);
                    gripPosition.y = std::min(levelLimits.height, grippablePosition.y);

                    gripperTransformable.draftTransform.setPosition(gripPosition);
                    gripperCollideable.draftBoxCollideable.SetCenter(gripPosition);
                }
            }
        }
    }

    if (!hasCollided) {
        world.RemoveGameEvent(GRIP_FEELING);
    }
}

void GripSystem::UpdateFeeling(World& world, Entity gripperEntity, Entity grippableEntity)
{
    Feel* gripperFeel = world.GetComponentIfExists<Feel>(gripperEntity);
    Feel* grippableFeel = world.GetComponentIfExists<Feel>(grippableEntity);

    if (gripperFeel != nullptr && grippableFeel != nullptr) {
        if (gripperFeel->feeling != grippableFeel->feeling) {
            gripperFeel->feeling = grippableFeel->feeling;

            Transformable& grippableTransformable = world.GetComponent<Transformable>(grippableEntity);
            float gripY = grippableTransformable.transform.getPosition().y;

            Feeling gripFeeling(gripperFeel->feeling);

            world.AddGameEvent(GRIP_FEELING, Event(gripFeeling, true));
        }

        ImGui::Begin("Feeling Infos");
        ImGui::Text("Lightball feeling: %d", grippableFeel->feeling);
        ImGui::Text("Character feeling: %d", gripperFeel->feeling);
        ImGui::End();
    }
}
