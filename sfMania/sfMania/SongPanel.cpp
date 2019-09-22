#include "SongPanel.h"

#include "Song.h"
#include "ResourceManager.h"
#include "SongSelectScene.h"
#include "GameManager.h"
#include "Maths.h"
#include "Easings.hpp"
#include "Utility.h"

SongPanel::SongPanel()
{
	m_songName = new sf::Text("", *ResourceManager::GetFont(eFont::big), 15);
	m_songName->setFillColor(sf::Color::White);
}


SongPanel::~SongPanel()
{
	delete m_songName;
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
			float y = Maths::Lerp(m_startY, m_targetY, easing);
			MediaBox::SetPosition(m_x, y);
		}
	}
}

void SongPanel::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(*m_songName);
}




//--------------------------
// MediaBox
//--------------------------

void SongPanel::OnMediaBoxSetPosition()
{
	m_songName->setPosition(m_x + 5, m_y + 5);
}




//--------------------------
// SongPanel
//--------------------------

void SongPanel::SetInformation(Song* song)
{
	m_songName->setString(song->m_title);
}

void SongPanel::Scroll(eScrollDir dir, float targetTime, float moveAmount)
{
	m_scrolling = true;
	m_elapsedScrollTime = 0.0f;
	m_targetScrollTime = targetTime;
	m_startY = m_y;
	m_targetY = (dir == eScrollDir::up) ? m_y - moveAmount : m_y + moveAmount;
}

void SongPanel::Highlight()
{
	MediaBox::SetFillColour(Utility::HighlightedColour());
}

void SongPanel::UnHighlight()
{
	MediaBox::SetFillColour(Utility::UnhighlightedColour());
}