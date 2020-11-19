#pragma once
#include "GameObject.h"

class Animation;

class HealthBar : public GameObject
{
public:
	HealthBar(int maxHealth);

	// GameObject
	void RenderSelf(sf::RenderWindow* window);
	void Update();
	void OnSetPosition();

	// HealthBar
	void SetupPosition();
	void IncreaseHealth(int health);
	void ReduceHealth(int health);
	bool IsDead() const;

private:

	const float MIN_WIDTH = 40;

	int m_health;
	int m_maxHealth;

	float m_barWidth;
	float m_currentHealthWidth;
	float m_currentLerpWidth;
	float m_height;

	float m_normalizedHealth;

	sf::RectangleShape m_healthBarBack;
	Animation* m_anim;

	void CalculateNewWidth();
};