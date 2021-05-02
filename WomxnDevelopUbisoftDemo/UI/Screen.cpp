#include "stdafx.h"

#include "Screen.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

Screen::Screen(const std::string& string, const std::string& backgroundTexturePath, const sf::Vector2u& appWindowSize)
{
    if (font.loadFromFile("Assets/arial.ttf")) {
        std::cout << "Unable to load font" << std::endl;
    }
    message.setFont(font);
    message.setString(string);
    message.setCharacterSize(50);
    message.setFillColor(sf::Color::White);
    message.setStyle(sf::Text::Bold);
    message.move(sf::Vector2f { (appWindowSize.x / 2.f) - 200.f, appWindowSize.y / 2.f });

    if (!backgroundTexture.loadFromFile(backgroundTexturePath)) {
        std::cout << "Unable to load asset" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.scale(sf::Vector2f { 6.f, 6.f });
}

void Screen::Draw(sf::RenderTarget& target) const
{
    target.draw(backgroundSprite);
    target.draw(message);
}
