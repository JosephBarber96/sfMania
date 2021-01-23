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
	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }
	void SetOutlineColour(sf::Color col);
	void SetOutlineThickness(float thickness);
	void SetFillColour(sf::Color col);
	void SetSize(int width, int height);
	virtual void OnMediaBoxSetPosition() = 0;

protected:
	sf::Color m_fillCol;
	sf::Color m_outlineCol;
	sf::RectangleShape m_box;
	int m_width;
	int m_height;
};

