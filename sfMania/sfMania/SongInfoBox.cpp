#include "SongInfoBox.h"

#include "Song.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "BPM.h"

SongInfoBox::SongInfoBox()
{
	int bf = 35;	// Big font size
	int sf = 25;	// Small font size
	int outlineThickness = 2;

	// Artist
	m_artistHeading = new sf::Text();
	Utility::SetupText(m_artistHeading, ResourceManager::GetFont(eFont::big), "Artist: ", bf, sf::Color::White, sf::Color::Black, outlineThickness);
	m_artist = new sf::Text();
	Utility::SetupText(m_artist, ResourceManager::GetFont(eFont::small), "@", sf, sf::Color::White, sf::Color::Black, outlineThickness);

	// Song
	m_songHeading = new sf::Text();
	Utility::SetupText(m_songHeading, ResourceManager::GetFont(eFont::big), "Song: ", bf, sf::Color::White, sf::Color::Black, outlineThickness);
	m_song = new sf::Text();
	Utility::SetupText(m_song, ResourceManager::GetFont(eFont::small), "@", sf, sf::Color::White, sf::Color::Black, outlineThickness);

	//BPM
	m_bpmHeading = new sf::Text();
	Utility::SetupText(m_bpmHeading, ResourceManager::GetFont(eFont::big), "BPM: ", bf, sf::Color::White, sf::Color::Black, outlineThickness);
	m_bpm = new sf::Text();
	Utility::SetupText(m_bpm, ResourceManager::GetFont(eFont::small), "@", sf, sf::Color::White, sf::Color::Black, outlineThickness);
}


SongInfoBox::~SongInfoBox()
{
}




//--------------------------
// GameObject
//--------------------------

void SongInfoBox::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(*m_artistHeading);
	window->draw(*m_artist);
	window->draw(*m_songHeading);
	window->draw(*m_song);
	window->draw(*m_bpmHeading);
	window->draw(*m_bpm);
}

void SongInfoBox::OnMediaBoxSetPosition()
{
	int pd = 10; // padding
	
	int small_gap = 30;
	int big_gap = 35;
	int y;

	y = pd;

	m_artistHeading->setPosition(m_x + pd, m_y + y);
	y += small_gap;
	m_artist->setPosition(m_x + pd, m_y + y);
	y += big_gap;

	m_songHeading->setPosition(m_x + pd, m_y + y);
	y += small_gap;
	m_song->setPosition(m_x + pd, m_y + y);
	y += big_gap;

	m_bpmHeading->setPosition(m_x + 5, m_y + y);
	y += small_gap;
	m_bpm->setPosition(m_x + pd, m_y + y);
	y += big_gap;
}




//--------------------------
// SongInfoBox
//--------------------------

void SongInfoBox::UpdateInformation(Song* song)
{
	m_artist->setString(song->m_artist);
	m_song->setString(song->m_title);
	if ((int)song->GetMinBpm() == (int)song->GetMaxBpm())
		m_bpm->setString(std::to_string((int)song->GetMinBpm()));
	else
		m_bpm->setString(std::to_string((int)song->GetMinBpm()) + " - " + std::to_string((int)song->GetMaxBpm()));
}