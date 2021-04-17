#pragma once

class StartScreen {
public:
    StartScreen(std::string message);
    void DrawScreen(sf::RenderTarget& target);

private:
    sf::Font font;
    sf::Text startText;
};
