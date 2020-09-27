#pragma once
#include "GameObject.h"

class ProgressBar : public GameObject
{
public:
	ProgressBar();

	// GameObject
	void RenderSelf(sf::RenderWindow* window);
	void Update();
	void OnSetPosition();

	// Progress bar
	void SetupPosition();
	void SetProgressNormalised(float normalisedProgress);

private:

	float m_left;
	float m_right;
	float m_yPos;

	sf::RectangleShape m_leftBar;
	sf::RectangleShape m_rightBar;
	sf::RectangleShape m_middleLine;

	sf::RectangleShape m_progressArrow;
};