#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();

	static std::vector<GameObject*> GameObjects;
	static void Init();

	virtual void Start();
	virtual void Update();
	virtual void RenderSelf(sf::RenderWindow* window) = 0;

	void SetPosition(float x, float y);
	virtual void OnSetPosition();

protected:
	float m_x;
	float m_y;
};