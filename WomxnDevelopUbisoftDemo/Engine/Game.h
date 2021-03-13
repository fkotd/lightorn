#pragma once

class Game
{
public:
    Game(const char* windowTitle);
    virtual ~Game();

    void RunGameLoop();

protected:
    virtual void Update(float deltaTime) = 0;
    virtual void Render(sf::RenderTarget& target) = 0;
    virtual void RenderDebugMenu(sf::RenderTarget& target) = 0;

private:
    sf::RenderWindow m_Window;
};