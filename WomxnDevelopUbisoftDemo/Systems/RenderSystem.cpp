#include "stdafx.h"

#include "RenderSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Darkness.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transformable.hpp"
#include "Tools/Layer.hpp"

void RenderSystem::Render(World& world, sf::RenderTarget& target)
{
    std::set<Entity> entities = world.Find(GetSignature());

    // TODO: move in a function
    Signature cameraCenterSignature = *world.GetSignature<CameraCenter>();
    std::set<Entity> cameraCenterEntities = world.Find(cameraCenterSignature);
    auto it = cameraCenterEntities.begin();
    Entity characterEntity = *it;

    ImGui::Begin("Render Menu");
    ImGui::Text("Number of entities: %d", entities.size());

    for (int layer = Layer::Back; layer < Layer::Top + 1; layer++) {
        RenderLayer(world, target, entities, static_cast<Layer>(layer), characterEntity);
    }

    ImGui::End();
}

void RenderSystem::RenderLayer(World& world, sf::RenderTarget& target, const std::set<Entity>& entities, Layer layer, Entity characterEntity)
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
            //Transformable* characterTransformable = world.GetComponentIfExists<Transformable>(characterEntity);
            //sf::Vector2f characterPosition = characterTransformable->transformable.getPosition();

            //darkness->shape->setPosition(characterPosition);
            //sf::Vector2f obscurityShapePosition = darkness->shape->getPosition();
            //ImGui::Text("Obscurity shape position %f, %f", obscurityShapePosition.x, obscurityShapePosition.y);

            //RigidBody* characterRigidBody = world.GetComponentIfExists<RigidBody>(characterEntity);
            //sf::Vector2f characterVelocity = characterRigidBody->velocity;

            //if (abs(characterVelocity.y) <= 2.f) {
            //    darkness->centerRadius = std::max(0.f, darkness->centerRadius - 2.f);
            //    darkness->middleRadius = std::max(0.f, darkness->middleRadius - 1.5f);
            //    darkness->outerRadius = std::max(0.f, darkness->outerRadius - 1.f);
            //} else {
            //    darkness->centerRadius = std::min(darkness->centerRadius + 1.f, 300.f);
            //    darkness->middleRadius = std::min(darkness->middleRadius + 1.5f, 700.f);
            //    darkness->outerRadius = std::min(darkness->outerRadius + 2.0f, 900.f);
            //}

            //darkness->shader->setUniform("obscurityColor", sf::Glsl::Vec4(darkness->shape->getFillColor()));
            //darkness->shader->setUniform("center", obscurityShapePosition);
            //darkness->shader->setUniform("centerRadius", darkness->centerRadius);
            //darkness->shader->setUniform("middleRadius", darkness->middleRadius);
            //darkness->shader->setUniform("outerRadius", darkness->outerRadius);

            target.draw(*darkness->shape, darkness->shader);
        }
    }
}
