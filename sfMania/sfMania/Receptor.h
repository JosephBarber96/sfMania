#pragma once

#include "GameObject.h"

class Receptor : public GameObject
{
public:
	Receptor();
	~Receptor();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow * window);
	void OnSetPosition();

	// Receptor
	void InitSelf(int column, int xPos);
	void Pressed();
	void Released();

	// Static
	static float GetReceptorY();

private:
	// Receptor
	sf::Texture * m_textureDown;
	sf::Texture * m_textureUp;
	sf::Sprite * m_sprite;

	// Press logic
	bool m_pressed;
	bool m_pushDown;
	float m_elapsedPushTime;

	// Static
	static Receptor * Left;
	static Receptor * LeftMid;
	static Receptor * RightMid;
	static Receptor * Right;
};