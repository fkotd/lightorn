#pragma once

#include "Tools/Layer.hpp"

struct Renderable {
    sf::Shape* shape;
    sf::Color color;
    sf::Vector2f size;
    Layer layer;
};