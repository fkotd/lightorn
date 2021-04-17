#include "stdafx.h"

#include "StartScreen.hpp"

#include <iostream>

StartScreen::StartScreen(std::string message)
{
    if (font.loadFromFile("Assets/arial.ttf")) {
        std::cout << "Unable to load font" << std::endl;
    }
    startText.setFont(font);
    startText.setString(message);
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);

    std::cout << "Start screen constructed" << std::endl;
}

void StartScreen::DrawScreen(sf::RenderTarget& target)
{
    target.draw(startText);
}
