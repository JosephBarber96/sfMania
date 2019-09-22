#pragma once

#include "GameObject.h"

class GameplayScene;

class Note : public GameObject
{
public:
	Note();
	~Note();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow * window);
	void OnSetPosition();

	// Note
	static void InitSceneRef(GameplayScene* scene);
	void BeginDrop(int x, int column);
	void EndDrop();

	bool IsActive() const { return m_active; }
	int Column() const { return m_currentColumn; }
	float ElapsedFallTime() const { return m_elapsedFallTime; }

protected:

	static GameplayScene* gameplayScene;

	// Note
	void Activate();
	virtual void OnActivate() {}

	// Sprite
	sf::Sprite * m_sprite;

	// If active
	bool m_active;

	// Falling vars
	float m_elapsedFallTime;
	float m_targetFallTime;
	float m_startY;
	float m_targetY;

	// Note size
	int m_width;
	int m_height;

	// Column
	int m_currentColumn;
};