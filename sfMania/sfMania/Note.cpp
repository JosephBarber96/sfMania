#include <string>
#include <iostream>

#include "Note.h"
#include "Receptor.h"

#include "GameManager.h"
#include "AssetManager.h"
#include "Settings.h"
#include "Maths.h"
#include "GameplayScene.h"


Note::Note()
{
	m_noteType = eNoteType::simple;
}

Note::~Note()
{
	
}




//--------------------------
// GameObject
//--------------------------

void Note::Update()
{
	if (!m_active) { return; }
	if (gameplayScene->SongPaused()) { return; }

	m_elapsedFallTime += GameManager::DeltaTime();
	float normalized = m_elapsedFallTime / m_targetFallTime;
	
	float y = Maths::Lerp(m_startY, m_targetY, normalized);
	SetPosition(m_x, y);

	// Note missed
	if ((m_elapsedFallTime - m_targetFallTime) > Settings::MissWindow())
	{
		Note::gameplayScene->AlertNoteMissed();
		EndNode();
	}
}

void Note::RenderSelf(sf::RenderWindow * window)
{
	if (!m_active) { return; }

	window->draw(*m_sprite);
}

void Note::OnSetPosition()
{
	m_sprite->setPosition(m_x, m_y);
}


//--------------------------
// BaseNote
//--------------------------

void Note::NoteHit()
{
	EndNode();
}


void Note::Activate()
{
	BaseNote::Activate();
}