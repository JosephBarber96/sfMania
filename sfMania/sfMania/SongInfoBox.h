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
	sf::Text* m_artistHeading;
	sf::Text* m_artist;

	sf::Text* m_songHeading;
	sf::Text* m_song;

	sf::Text* m_bpmHeading;
	sf::Text* m_bpm;
};