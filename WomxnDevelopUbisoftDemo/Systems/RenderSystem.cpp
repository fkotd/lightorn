#include "stdafx.h"

#include "RenderSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Darkness.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transformable.hpp"
#include "Core/Signature.hpp"
#include "Tools/Layer.hpp"
#include <SFML/Graphics/View.hpp>

void RenderSystem::Render(World& world, sf::RenderTarget& target)
{
    std::set<Entity> entities = world.Find(GetSignature());

    ImGui::Begin("Render Menu");
    ImGui::Text("Number of entities: %d", entities.size());
    ImGui::End();

    for (int layer = Layer::Back; layer < Layer::Top + 1; layer++) {
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

        // If the current entity has the camera component
        CameraCenter* cameraCenter = world.GetComponentIfExists<CameraCenter>(entity);
        if (cameraCenter != nullptr) {
            // It is the player
            Transformable& transformable = world.GetComponent<Transformable>(entity);
            // Update the view
            sf::View view = target.getView();
            view.setCenter(view.getCenter().x, transformable.transformable.getPosition().y);
            target.setView(view);
        }

        // If the current entity has a transformable component, update its shape
        Transformable* transformable = world.GetComponentIfExists<Transformable>(entity);
        Sprite* sprite = world.GetComponentIfExists<Sprite>(entity);
        Darkness* darkness = world.GetComponentIfExists<Darkness>(entity);

        if (transformable != nullptr) {
            if (renderable.shape != nullptr) {
                renderable.shape->setPosition(transformable->transformable.getPosition());
            }

            if (sprite != nullptr) {
                sprite->sprite->setPosition(transformable->transformable.getPosition());
            }
        }

        if (renderable.shape != nullptr) {
            target.draw(*renderable.shape);
        }

        if (sprite != nullptr) {
            target.draw(*sprite->sprite);
        }

        if (darkness != nullptr && transformable != nullptr) {
            target.draw(*darkness->shape, darkness->shader);
        }
    }
}
