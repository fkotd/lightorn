#pragma once

#include "Tools/Layer.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>

struct Renderable {
    sf::Shape* shape;
    sf::Color color;
    sf::Vector2f size;
    Layer layer;
};