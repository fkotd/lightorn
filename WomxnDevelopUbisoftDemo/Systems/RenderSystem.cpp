#include "stdafx.h"

#include "RenderSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transformable.hpp"
#include "Tools/Layer.hpp"

void RenderSystem::Render(World& world, sf::RenderTarget& target)
{
    std::set<Entity> entities = world.Find(GetSignature());

    ImGui::Begin("Render Menu");
    ImGui::Text("Number of entities: %d", entities.size());
    ImGui::End();

    target.clear(sf::Color(0, 0, 0));

    for (int layer = Layer::Top; layer < Layer::Back + 1; layer++) {
        RenderLayer(world, target, entities, static_cast<Layer>(layer));
    }
}

void RenderSystem::RenderLayer(World& world, sf::RenderTarget& target, const std::set<Entity>& entities, Layer layer)
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
            view.setCenter(view.getCenter().x, transformable.transform.getPosition().y);
            target.setView(view);
        }

        // if the current entity has a transformable component, update its shape
        Transformable* transformable = world.GetComponentIfExists<Transformable>(entity);
        if (transformable != nullptr) {
            if (renderable.shape != nullptr) {
                renderable.shape->setPosition(transformable->transform.getPosition());
            }

            if (renderable.sprite != nullptr) {
                renderable.sprite->setPosition(transformable->transform.getPosition());
            }
        }

        if (renderable.shape != nullptr) {
            target.draw(*renderable.shape);
        }

        if (renderable.sprite != nullptr) {
            target.draw(*renderable.sprite);
        }
    }
}
