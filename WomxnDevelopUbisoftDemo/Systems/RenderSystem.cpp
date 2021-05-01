#include "stdafx.h"

#include "RenderSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Collideable.hpp"
#include "Components/Obscurity.hpp"
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
            view.setCenter(view.getCenter().x, transformable.transform.getPosition().y);
            target.setView(view);
        }

        // If the current entity has a transformable component, update its shape
        Transformable* transformable = world.GetComponentIfExists<Transformable>(entity);
        Sprite* sprite = world.GetComponentIfExists<Sprite>(entity);
        Obscurity* obscurity = world.GetComponentIfExists<Obscurity>(entity);

        if (transformable != nullptr) {
            if (renderable.shape != nullptr) {
                renderable.shape->setPosition(transformable->transform.getPosition());
            }

            if (sprite != nullptr) {
                sprite->sprite->setPosition(transformable->transform.getPosition());
            }
        }

        if (renderable.shape != nullptr) {
            target.draw(*renderable.shape);
        }

        if (sprite != nullptr) {
            target.draw(*sprite->sprite);
        }

        if (obscurity != nullptr && transformable != nullptr) {
            Transformable* characterTransformable = world.GetComponentIfExists<Transformable>(characterEntity);
            sf::Vector2f characterPosition = characterTransformable->transform.getPosition();

            obscurity->shape->setPosition(characterPosition);
            sf::Vector2f obscurityShapePosition = obscurity->shape->getPosition();
            ImGui::Text("Obscurity shape position %f, %f", obscurityShapePosition.x, obscurityShapePosition.y);

            RigidBody* characterRigidBody = world.GetComponentIfExists<RigidBody>(characterEntity);
            sf::Vector2f characterVelocity = characterRigidBody->velocity;

            if (abs(characterVelocity.y) <= 2.f) {
                obscurity->centerRadius -= 1.f;
                obscurity->middleRadius -= 1.f;
            } else {
                obscurity->centerRadius += 1.f;
                obscurity->middleRadius += 1.f;
            }

            obscurity->shader->setUniform("obscurityColor", sf::Glsl::Vec4(obscurity->shape->getFillColor()));
            obscurity->shader->setUniform("center", obscurityShapePosition);
            obscurity->shader->setUniform("centerRadius", obscurity->centerRadius);
            obscurity->shader->setUniform("middleRadius", obscurity->middleRadius);

            target.draw(*obscurity->shape, obscurity->shader);
        }
    }
}
