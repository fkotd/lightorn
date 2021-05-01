#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>

namespace sf {
class RenderTarget;
}

class Screen {
public:
    Screen(std::string string, std::string backgroundTexturePath);
    void Draw(sf::RenderTarget& target) const;

private:
    sf::Font font;
    sf::Text message;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
};
