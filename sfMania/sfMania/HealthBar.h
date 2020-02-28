#pragma once
#include "GameObject.h"

class HealthBar : public GameObject
{
public:
	HealthBar(int maxHealth);

	// GameObject
	void RenderSelf(sf::RenderWindow* window);
	void Update();
	void OnSetPosition();

	// HealthBar
	void IncreaseHealth(int health);
	void ReduceHealth(int health);
	bool IsDead() const;

private:

	const float MIN_WIDTH = 40;

	int m_health;
	int m_maxHealth;

	float m_width;
	float m_realWidth;
	float m_maxWidth;
	float m_height;

	sf::RectangleShape m_healthBarBack;
	sf::RectangleShape m_healthBar;

	void CalculateNewWidth();
};