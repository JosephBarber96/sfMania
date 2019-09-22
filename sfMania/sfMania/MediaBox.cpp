#include "MediaBox.h"



MediaBox::MediaBox()
{
	m_box = sf::RectangleShape();
}


MediaBox::~MediaBox()
{
}




//--------------------------
// GameObject
//--------------------------

void MediaBox::RenderSelf(sf::RenderWindow* window)
{
	window->draw(m_box);
}

void MediaBox::OnSetPosition()
{
	m_box.setPosition(m_x, m_y);
	OnMediaBoxSetPosition();
}




//--------------------------
// MediaBox
//--------------------------

void MediaBox::SetOutlineColour(sf::Color col)
{
	m_box.setOutlineColor(col);
}

void MediaBox::SetOutlineThickness(float thickness)
{
	m_box.setOutlineThickness(thickness);
}

void MediaBox::SetFillColour(sf::Color col)
{
	m_box.setFillColor(col);
}

void MediaBox::SetSize(int width, int height)
{
	m_box.setSize(sf::Vector2f(width, height));
}