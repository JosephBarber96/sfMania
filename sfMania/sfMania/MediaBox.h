#pragma once
#include "GameObject.h"

class MediaBox : public GameObject
{
public:
	MediaBox();
	~MediaBox();

	// GameObject
	void RenderSelf(sf::RenderWindow* window);
	void OnSetPosition();

	// MediaBox
	void SetOutlineColour(sf::Color col);
	void SetOutlineThickness(float thickness);
	void SetFillColour(sf::Color col);
	void SetSize(int width, int height);
	virtual void OnMediaBoxSetPosition() = 0;

protected:
	sf::RectangleShape m_box;
};

