#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>

namespace sf {
class RenderTarget;
}

class StartScreen {
public:
    StartScreen(std::string message);
    void DrawScreen(sf::RenderTarget& target) const;

private:
    sf::Font font; // faire un include par champs
    sf::Text startText;
    sf::Sprite backgroundSprite;
    sf::Texture texture;
};
