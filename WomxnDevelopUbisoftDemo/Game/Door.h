#pragma once

class Door : public sf::Drawable, public BoxCollideable
{
public:
	Door(float xCenterPos, float yCenterPos, float width, float height);
	~Door();

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void StartEndGame();

private:
    sf::RectangleShape m_Rectangle;

	float m_rColor;
	float m_gColor;
	float m_bColor;

	bool m_IsPlayingEndGame;
};