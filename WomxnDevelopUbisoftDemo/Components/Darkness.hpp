#pragma once

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Shape.hpp>

struct Darkness {
    sf::Shape* shape;
    sf::Shader* shader;
    float centerRadius;
    float middleRadius;
    float outerRadius;
};
