#include "ProgressBar.h"

#include "Settings.h"
#include "Maths.h"

ProgressBar::ProgressBar()
{
	m_leftBar = sf::RectangleShape();
	m_rightBar = sf::RectangleShape();
	m_middleLine = sf::RectangleShape();
	
	sf::Color col = sf::Color::White;

	m_leftBar.setFillColor(col);
	m_rightBar.setFillColor(col);
	m_middleLine.setFillColor(col);
	m_progressArrow.setFillColor(col);
}


void ProgressBar::RenderSelf(sf::RenderWindow* window)
{
	window->draw(m_leftBar);
	window->draw(m_rightBar);
	window->draw(m_middleLine);
	window->draw(m_progressArrow);
}

void ProgressBar::Update()
{

}

void ProgressBar::OnSetPosition()
{

}

// -- 

void ProgressBar::SetupPosition()
{
	float screenWidth = Settings::WindowX();

	m_left = screenWidth * 0.15f;
	m_right = screenWidth * 0.85f;
	m_yPos = 15;
	float barLength = screenWidth * 0.7f;
	float barThickness = 2;
	float barHeight = 15;

	m_leftBar.setSize(sf::Vector2f(barThickness, barHeight));
	m_rightBar.setSize(sf::Vector2f(barThickness, barHeight));
	m_leftBar.setPosition(m_left, m_yPos);
	m_rightBar.setPosition(m_right, m_yPos);
	
	m_middleLine.setSize(sf::Vector2f(barLength, barThickness));
	m_middleLine.setPosition(m_left, float(m_yPos + (barHeight/2.0f)));

	m_progressArrow.setSize(sf::Vector2f(barHeight / 2, barHeight / 2));
}

void ProgressBar::SetProgressNormalised(float normalisedProgress)
{
	float x = Maths::Lerp(m_left, m_right, normalisedProgress);
	m_progressArrow.setPosition(x, m_yPos);
}