#include "SongInfoBox.h"

#include "Song.h"
#include "AssetManager.h"
#include "Utility.h"
#include "BPM.h"

SongInfoBox::SongInfoBox()
{
	int bf = 35;	// Big font size
	int sf = 25;	// Small font size
	int outlineThickness = 2;

	// Artist
	m_songText = new sf::Text();
	Utility::SetupText(m_songText, AssetManager::GetFont(eFont::small), "@", sf, sf::Color::White, sf::Color::Black, outlineThickness);

	// BPM 
	m_bpmText = new sf::Text();
	Utility::SetupText(m_bpmText, AssetManager::GetFont(eFont::small), "@", sf, sf::Color::White, sf::Color::Black, outlineThickness);
}


SongInfoBox::~SongInfoBox()
{
	delete m_songText;
	delete m_bpmText;
}




//--------------------------
// GameObject
//--------------------------

void SongInfoBox::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(*m_songText);
	window->draw(*m_bpmText);
}

void SongInfoBox::OnMediaBoxSetPosition()
{
	int pd = 10; // padding
	
	int small_gap = 30;
	int big_gap = 35;
	int y;

	y = pd;

	// Song
	m_songText->setPosition(m_x + pd, m_y + y);
	y += big_gap;

	// BPM
	m_bpmText->setPosition(m_x + pd, m_y + y);
}




//--------------------------
// SongInfoBox
//--------------------------

void SongInfoBox::UpdateInformation(Song* song)
{
	m_songText->setString(song->m_artist + "  :  " + song->m_title);

	if ((int)song->GetMinBpm() == (int)song->GetMaxBpm())
		m_bpmText->setString("BPM  :  " + std::to_string((int)song->GetMinBpm()));
	else
		m_bpmText->setString("BPM  :  " + std::to_string((int)song->GetMinBpm()) + " - " + std::to_string((int)song->GetMaxBpm()));
}