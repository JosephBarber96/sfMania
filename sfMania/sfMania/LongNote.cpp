#include <iostream>

#include "LongNote.h"

#include "GameManager.h"
#include "Maths.h"
#include "Receptor.h"
#include "GameplayScene.h"

LongNote::LongNote()
	:
	Note()
{
	m_longBar = new sf::RectangleShape();
	m_longBar->setFillColor(sf::Color(255, 255, 255, 128));
}


LongNote::~LongNote()
{
	delete m_longBar;
}



//--------------------------
// GameObject
//--------------------------

void LongNote::Update()
{
	if (!m_active) { return; }
	if (gameplayScene->SongPaused()) { return; }

	// Fall time
	m_elapsedFallTime += GameManager::DeltaTime();
	float normalized = m_elapsedFallTime / m_targetFallTime;

	// Normalized fall pos
	float y = Maths::Lerp(m_startY, m_targetY, normalized);
	SetPosition(m_x, y);

	if (m_readyToEnd)
	{
		EndDrop();
	}
}

void LongNote::RenderSelf(sf::RenderWindow * window)
{
	if (!m_active) { return; }

	window->draw(*m_longBar);
	window->draw(*m_sprite);
}

void LongNote::OnSetPosition()
{
	// Sprite
	float spriteY = Maths::Clamp(m_y, 0, Receptor::GetReceptorY());
	m_sprite->setPosition(m_x, spriteY);
		

	// Long note
	switch (m_longNoteMode)
	{
	case eLongNoteMode::expanding:
		// Count how long we've been lengthening for
		m_elapsedExpandTime += GameManager::DeltaTime();

		// Extend our size
		m_height = m_y + m_sprite->getGlobalBounds().height / 2.0f;
		m_longBar->setPosition(m_x, 0);
		m_longBar->setSize(sf::Vector2f(m_sprite->getGlobalBounds().width, m_height));

		// If we reach the receptor, go to Still
		if (m_height >= (Receptor::GetReceptorY()))
			SetLongnoteMode(eLongNoteMode::still);
		break;

	case eLongNoteMode::still:
		// Do nothing
		break;

	case eLongNoteMode::falling:
		m_longBar->setPosition(m_x, m_y - m_endExpandHeight);

		if (m_y >= Receptor::GetReceptorY())
			SetLongnoteMode(eLongNoteMode::shrinking);
		break;

	case eLongNoteMode::shrinking:
		m_elapsedShrinkTime += GameManager::DeltaTime();
		float normalized = m_elapsedShrinkTime / m_targetShrinkTime;

		// Lerp size from height to 0
		float newHeight = Maths::Lerp(m_height, 0, normalized);

		// Set pos y at target and set size inverse lerped height (shrink downwards)
		m_longBar->setPosition(m_x, Receptor::GetReceptorY());
		m_longBar->setSize(sf::Vector2f(m_sprite->getGlobalBounds().width, -newHeight));

		if (normalized >= 1)
		{
			m_readyToEnd = true;
		}	
		break;
	}	
}




//--------------------------
// LongNote
//--------------------------

void LongNote::OnActivate()
{
	SetLongnoteMode(eLongNoteMode::expanding);
}

void LongNote::SetLongnoteMode(eLongNoteMode newMode)
{
	m_longNoteMode = newMode;
	switch (m_longNoteMode)
	{
	case eLongNoteMode::expanding:
		m_longBar->setSize(sf::Vector2f(0, 0));
		m_height = 0.0f;
		m_elapsedExpandTime = 0.0f;
		m_endExpandHeight = 0.0f;
		m_elapsedShrinkTime = 0.0f;
		m_targetShrinkTime = 0.0f;
		m_readyToEnd = false;
		break;

	case eLongNoteMode::still:
		m_targetShrinkTime = m_elapsedExpandTime;
		break;

	case eLongNoteMode::falling:
		m_endExpandHeight = m_y;
		break;

	case eLongNoteMode::shrinking:
		m_targetShrinkTime = m_elapsedExpandTime;
		m_elapsedShrinkTime = 0.0f;
		break;
	}
}

void LongNote::EndTail()
{
	// If we're still expanding, we start falling
	if (m_longNoteMode == eLongNoteMode::expanding)
		SetLongnoteMode(eLongNoteMode::falling);
		

	// If we were fully expanded, we start shrinking
	else if (m_longNoteMode == eLongNoteMode::still)
		SetLongnoteMode(eLongNoteMode::shrinking);
}