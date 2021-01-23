#include "SongPanel.h"

#include "Song.h"
#include "AssetManager.h"
#include "SongSelectScene.h"
#include "GameManager.h"
#include "Maths.h"
#include "Easings.hpp"
#include "Utility.h"

SongPanel::SongPanel()
{
	m_songName = sf::Text("", *AssetManager::GetFont(eFont::big), 15);
	m_songName.setFillColor(sf::Color::White);
	m_texture = sf::Texture();
	m_sprite = sf::Sprite();
}


SongPanel::~SongPanel()
{

}




//--------------------------
// GameObject
//--------------------------

void SongPanel::Update()
{
	if (m_scrolling)
	{
		m_elapsedScrollTime += GameManager::DeltaTime();

		if (m_elapsedScrollTime >= m_targetScrollTime)
		{
			m_scrolling = false;
			MediaBox::SetPosition(m_x, (int)m_targetY);
		}
		else
		{
			float normalized = m_elapsedScrollTime / m_targetScrollTime;
			float easing = Easings::Quad::Out(normalized);
			float x = Maths::Lerp(m_startX, m_targetX, easing);
			float y = Maths::Lerp(m_startY, m_targetY, easing);	
			MediaBox::SetPosition(x, y);
		}
	}
}

void SongPanel::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);
	window->draw(m_sprite);
	window->draw(m_songName);
}




//--------------------------
// MediaBox
//--------------------------

void SongPanel::OnMediaBoxSetPosition()
{
	m_songName.setPosition(m_x + 5, m_y + 5);
	m_sprite.setPosition(m_x, m_y);
}




//--------------------------
// SongPanel
//--------------------------

void SongPanel::SetInformation(Song* song)
{
	m_songName.setString(song->m_title);
	m_texture.loadFromFile(song->m_backgroundFile);
	m_texture.setSmooth(true);
	m_sprite.setTexture(m_texture);
	m_sprite.setScale(Utility::GetScaleForTargetSize(&m_texture, SIZE, SIZE));
}

void SongPanel::Scroll(float targetTime, float moveX, float moveY)
{
	m_scrolling = true;
	m_elapsedScrollTime = 0.0f;
	m_targetScrollTime = targetTime;
	m_startX = m_x;
	m_targetX = m_x + moveX;
	m_startY = m_y;
	m_targetY = m_y + moveY;
}

void SongPanel::Highlight()
{
	MediaBox::SetFillColour(Utility::HighlightedColour());
	m_sprite.setColor(sf::Color::White);
}

void SongPanel::UnHighlight()
{
	MediaBox::SetFillColour(Utility::UnhighlightedColour());
	m_sprite.setColor(sf::Color(255, 255, 255, 64));
}