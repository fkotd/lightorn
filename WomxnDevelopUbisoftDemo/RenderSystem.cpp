#include "stdafx.h"
#include "RenderSystem.hpp"
#include "Renderable.hpp"
#include "CameraCenter.hpp"
#include "Transformable.hpp"
#include "Collideable.hpp"

void RenderSystem::Render(const std::unique_ptr<World>& world, sf::RenderWindow* window)
{
    std::set<Entity> entities = world->Find(GetSignature());

    window->clear(sf::Color(0, 0, 0));

    for (auto entity : entities) {
        // if the current entity has the camera component
        CameraCenter* cameraCenter = world->GetComponentIfExists<CameraCenter>(entity);
        if (cameraCenter != nullptr) {
            // this is player
            Transformable& transformable = world->GetComponent<Transformable>(entity);
            // update the view
            sf::View view = window->getView();
            view.setCenter(view.getCenter().x, transformable.transformable.getPosition().y);
            window->setView(view);
        }

        Renderable& renderable = world->GetComponent<Renderable>(entity);

        // if the current entity has a transformable component
        // update its shape
        Transformable* transformable = world->GetComponentIfExists<Transformable>(entity);
        if (transformable != nullptr) {
            // we should update the shape of the entity accordingly to its position
            renderable.shape->setPosition(transformable->transformable.getPosition());

            // if the current entity has a bounding box
            // update its position
            Collideable* collideable = world->GetComponentIfExists<Collideable>(entity);
            if (collideable != nullptr) {
                collideable->boxCollideable->SetCenter(transformable->transformable.getPosition());
            }
        }
       
        window->draw(*renderable.shape);
    }

    window->display();
}
