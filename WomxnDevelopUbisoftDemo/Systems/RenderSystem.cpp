#include "stdafx.h"

#include "RenderSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transformable.hpp"

void RenderSystem::Render(World& world, sf::RenderTarget& target)
{
    std::set<Entity> entities = world.Find(GetSignature());

    std::set<Entity> topLayerEntities;

    ImGui::Begin("Render Menu");
    ImGui::Text("Number of entities: %d", entities.size());
    ImGui::End();

    target.clear(sf::Color(0, 0, 0));

    RenderLayer(world, target, entities, 2);
    RenderLayer(world, target, entities, 1);
}

void RenderSystem::RenderLayer(World& world, sf::RenderTarget& target, const std::set<Entity>& entities, int layer)
{
    for (auto entity : entities) {
        Renderable& renderable = world.GetComponent<Renderable>(entity);
        if (renderable.layer != layer) {
            continue;
        }

        // if the current entity has the camera component
        CameraCenter* cameraCenter = world.GetComponentIfExists<CameraCenter>(entity);
        if (cameraCenter != nullptr) {
            // it is the player
            Transformable& transformable = world.GetComponent<Transformable>(entity);
            // update the view
            sf::View view = target.getView();
            view.setCenter(view.getCenter().x, transformable.transformable.getPosition().y);
            target.setView(view);
        }

        // if the current entity has a transformable component, update its shape
        Transformable* transformable = world.GetComponentIfExists<Transformable>(entity);
        if (transformable != nullptr) {
            renderable.shape->setPosition(transformable->transformable.getPosition());

            // if the current entity has a bounding box, update this position
            Collideable* collideable = world.GetComponentIfExists<Collideable>(entity);
            if (collideable != nullptr) {
                collideable->boxCollideable.SetCenter(transformable->transformable.getPosition());
            }
        }

        target.draw(*renderable.shape);
    }
}
