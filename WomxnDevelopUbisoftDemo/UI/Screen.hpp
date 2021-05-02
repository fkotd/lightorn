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
    Screen(const std::string& string, const std::string& backgroundTexturePath, const sf::Vector2u& appWindowSize);
    void Draw(sf::RenderTarget& target) const;

private:
    sf::Font font;
    sf::Text message;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
};
