#pragma once
#include "MediaBox.h"

class Song;

class SongInfoBox : public MediaBox
{
public:
	SongInfoBox();
	~SongInfoBox();

	// GameObject
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	void OnMediaBoxSetPosition();

	// SongInfoBox
	void UpdateInformation(Song* song);

private:
	sf::Text* m_songText;
	sf::Text* m_bpmText;
};