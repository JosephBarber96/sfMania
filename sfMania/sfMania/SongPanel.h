#pragma once

#include "MediaBox.h"

class Song;
enum eScrollDir;

class SongPanel : public MediaBox
{
public:
	SongPanel();
	~SongPanel();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	virtual void OnMediaBoxSetPosition();

	// SongPanel
	void SetInformation(Song* song);
	void Scroll(eScrollDir dir, float targetTime, float moveAmount);
	void Highlight();
	void UnHighlight();

protected:
	sf::Text* m_songName;

	// SongPanel
	bool m_scrolling;
	float m_elapsedScrollTime;
	float m_targetScrollTime;
	float m_startY;
	float m_targetY;
};