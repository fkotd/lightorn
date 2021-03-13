#pragma once

#include <Engine/Game.h>
#include <Game/Door.h>
#include <Game/MainCharacter.h>

class GameDemo : public Game
{
public:
    GameDemo();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

private:
    sf::Font m_EndgameTextFont;
    sf::Text m_EndgameText;
    sf::SoundBuffer m_EndgameSoundBuffer;
    sf::Sound m_EndgameSound;
    Door m_Door;
    MainCharacter m_MainCharacter;
    bool m_IsFinished;
};