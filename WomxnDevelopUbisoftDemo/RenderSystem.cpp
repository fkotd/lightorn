#include "stdafx.h"
#include "RenderSystem.hpp"
#include "Renderable.hpp"

void RenderSystem::Render(World* world, sf::RenderWindow* window)
{
    std::set<Entity> entities = world->Find(GetSignature());

    window->clear(sf::Color(0, 0, 0));

    for (auto entity : entities) {
        Renderable* renderable = world->GetComponent<Renderable>(entity);
        window->draw(*renderable->shape);
    }

    window->display();
}
