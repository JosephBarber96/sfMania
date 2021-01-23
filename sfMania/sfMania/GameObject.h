#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	static std::vector<GameObject*> GameObjects;
	static void Init();

	virtual void Update();
	virtual void RenderSelf(sf::RenderWindow* window) = 0;

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	void SetPosition(float x, float y);
	void SetPositionNormalized(float x, float y);
	virtual void OnSetPosition();

	void SetActive(bool active);
	bool IsActive() const;

protected:
	float m_x;
	float m_y;
	bool m_active;
};