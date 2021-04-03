#include "stdafx.h"

#include "RenderSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transformable.hpp"

void RenderSystem::Render(World& world, sf::RenderTarget& target)
{
    std::set<Entity> entities = world.Find(GetSignature());

    ImGui::Begin("Render Menu");
    ImGui::Text("Number of entities: %d", entities.size());
    ImGui::End();

    target.clear(sf::Color(0, 0, 0));

    for (auto entity : entities) {
        // if the current entity has the camera component
        CameraCenter* cameraCenter = world.GetComponentIfExists<CameraCenter>(entity);
        if (cameraCenter != nullptr) {
            // this is player
            Transformable& transformable = world.GetComponent<Transformable>(entity);
            // update the view
            sf::View view = target.getView();
            view.setCenter(view.getCenter().x, transformable.transformable.getPosition().y);
            target.setView(view);
        }

        Renderable& renderable = world.GetComponent<Renderable>(entity);

        // if the current entity has a transformable component
        // update its shape
        Transformable* transformable = world.GetComponentIfExists<Transformable>(entity);
        if (transformable != nullptr) {
            // we should update the shape of the entity accordingly to its position
            renderable.shape->setPosition(transformable->transformable.getPosition());

            // if the current entity has a bounding box
            // update its position
            Collideable* collideable = world.GetComponentIfExists<Collideable>(entity);
            if (collideable != nullptr) {
                collideable->boxCollideable->SetCenter(transformable->transformable.getPosition());
            }
        }

        target.draw(*renderable.shape);
    }
}
