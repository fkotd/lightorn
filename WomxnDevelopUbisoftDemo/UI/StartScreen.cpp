#include "stdafx.h"

#include "StartScreen.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

StartScreen::StartScreen(std::string message)
{
    if (font.loadFromFile("Assets/arial.ttf")) {
        std::cout << "Unable to load font" << std::endl;
    }
    startText.setFont(font);
    startText.setString(message);
    startText.setCharacterSize(50);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);
    startText.move(sf::Vector2f { 600.f, 400.f }); // TODO: make it relative to window size ?

    if (!texture.loadFromFile("Assets/start_screen.png")) {
        std::cout << "Unable to load asset" << std::endl;
    }

    backgroundSprite.setTexture(texture);
    backgroundSprite.scale(sf::Vector2f { 6.1f, 6.1f }); // TODO: make it relative ?

    std::cout << "Start screen constructed" << std::endl;
}

void StartScreen::DrawScreen(sf::RenderTarget& target) const
{
    target.draw(backgroundSprite);
    target.draw(startText);
}
