#pragma once

#include "MediaBox.h"

class Song;

class SongPanel : public MediaBox
{
public:
	SongPanel();
	~SongPanel();

	static const int SIZE = 250;

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	virtual void OnMediaBoxSetPosition();

	// SongPanel
	void SetInformation(Song* song);
	void Scroll(float targetTime, float moveX, float moveY);
	void Highlight();
	void UnHighlight();

protected:

	sf::Text m_songName;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	// SongPanel
	bool m_scrolling;
	float m_elapsedScrollTime;
	float m_targetScrollTime;

	float m_startY;
	float m_targetY;
	float m_startX; 
	float m_targetX;
};