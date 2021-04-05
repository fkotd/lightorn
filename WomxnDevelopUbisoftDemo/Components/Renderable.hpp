#pragma once

struct Renderable {
    sf::Shape* shape;
    sf::Color color;
    sf::Vector2f size; // width and height
    int layer; // 1: front, 2: back
};