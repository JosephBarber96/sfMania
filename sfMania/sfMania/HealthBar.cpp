#include "HealthBar.h"

#include "Settings.h"
#include "Maths.h"
#include "GameManager.h"

#include <iostream>

HealthBar::HealthBar(int maxHealth)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth / 2;

	m_maxWidth = Settings::WindowX() * 0.75f;
	m_realWidth = m_maxWidth;

	m_height = Settings::WindowY() * 0.05f;
	if (m_height < MIN_WIDTH)
		m_health = MIN_WIDTH;

	m_healthBarBack = sf::RectangleShape();
	m_healthBarBack.setSize(sf::Vector2f(m_maxWidth, m_height));
	m_healthBarBack.setFillColor(sf::Color(20, 20, 20, 255));

	m_healthBar = sf::RectangleShape();
	m_healthBar.setPosition(sf::Vector2f(m_maxWidth, m_height));
	m_healthBar.setFillColor(sf::Color::Green);
	
	CalculateNewWidth();
}

void HealthBar::RenderSelf(sf::RenderWindow* window)
{
	window->draw(m_healthBarBack);
	window->draw(m_healthBar);
}

void HealthBar::Update()
{
	m_width = Maths::Lerp(m_width, m_realWidth, GameManager::DeltaTime() * 16.f);
	m_healthBar.setSize(sf::Vector2f(m_width, m_height));
}

void HealthBar::OnSetPosition()
{
	m_healthBarBack.setPosition(m_x, m_y);
	m_healthBar.setPosition(m_x, m_y);
}

// --

void HealthBar::SetupPosition()
{
	float middle = Settings::WindowX() / 2;
	float barWidth = m_maxWidth;

	float xPos = (middle - barWidth / 2);
	float yPos = Settings::WindowY() * 0.05f;

	m_healthBar.setPosition(xPos, yPos);
	m_healthBarBack.setPosition(xPos, yPos);
}

void HealthBar::IncreaseHealth(int health)
{
	m_health += health;
	if (m_health > m_maxHealth)
		m_health = m_maxHealth;
	CalculateNewWidth();
}

void HealthBar::ReduceHealth(int health)
{
	m_health -= health;
	if (m_health < 0)
		m_health = 0;
	CalculateNewWidth();
}

bool HealthBar::IsDead() const
{
	return m_health <= 0;
}

void HealthBar::CalculateNewWidth()
{
	float normalizedHealth = (float)m_health / (float)m_maxHealth;
	m_realWidth = Maths::Lerp(0, m_maxWidth, normalizedHealth);
}