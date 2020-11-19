#include "HealthBar.h"

#include "Settings.h"
#include "Maths.h"
#include "GameManager.h"
#include "AssetManager.h"
#include "Animation.h"

#include "Utility.h"

const float MAX_NORMALIZED_WIDTH = 0.75;
const float MAX_NORMALIZED_HEIGHT = 0.075f;

HealthBar::HealthBar(int maxHealth)
{
	m_maxHealth = maxHealth;
	m_health = m_maxHealth;

	m_barWidth = Settings::WindowX() * MAX_NORMALIZED_WIDTH;
	m_height = Settings::WindowY() * MAX_NORMALIZED_HEIGHT;
	m_currentHealthWidth = m_barWidth;

	m_healthBarBack = sf::RectangleShape();
	m_healthBarBack.setSize(sf::Vector2f(m_barWidth, m_height));
	m_healthBarBack.setFillColor(sf::Color(20, 20, 20, 255));
	m_healthBarBack.setOutlineThickness(2);
	m_healthBarBack.setOutlineColor(sf::Color::White);

	m_anim = Animation();
	m_anim.SetupAnimation(AssetManager::GetAnimation(eAnimation::health_bar));
	m_anim.SetSize(m_barWidth, m_height);
	m_anim.Play(true);
	
	CalculateNewWidth();
}

void HealthBar::RenderSelf(sf::RenderWindow* window)
{
	window->draw(m_healthBarBack);
	m_anim.RenderSelf(window);
}

void HealthBar::Update()
{
	m_currentLerpWidth = Maths::Lerp(m_currentLerpWidth, m_currentHealthWidth, GameManager::DeltaTime() * 16.f);
	m_anim.SetSize(m_currentLerpWidth, m_height);
	m_anim.SetColour(Utility::Lerp(sf::Color::Green, sf::Color::Red, 1 - m_normalizedHealth));
}

void HealthBar::OnSetPosition()
{
	m_healthBarBack.setPosition(m_x, m_y);
	m_anim.SetPosition(m_x, m_y);
}

// --

void HealthBar::SetupPosition()
{
	float middle = Settings::WindowX() / 2;

	float xPos = (middle - m_barWidth / 2);
	float yPos = Settings::WindowY() * 0.05f;

	m_healthBarBack.setPosition(xPos, yPos);
	m_anim.SetPosition(xPos, yPos);
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
	m_normalizedHealth = (float)m_health / (float)m_maxHealth;
	m_currentHealthWidth = Maths::Lerp(0, m_barWidth, m_normalizedHealth);
}