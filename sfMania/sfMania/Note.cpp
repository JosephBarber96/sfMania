#include <string>
#include <iostream>

#include "Note.h"
#include "Receptor.h"

#include "GameManager.h"
#include "AssetManager.h"
#include "Settings.h"
#include "Maths.h"
#include "GameplayScene.h"

GameplayScene* Note::gameplayScene;

Note::Note()
{
	m_sprite = new sf::Sprite();

	m_elapsedFallTime = 0.0f;
	m_targetFallTime = Settings::FallTime();
}

Note::~Note()
{
	delete m_sprite;
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
		EndDrop();
		Note::gameplayScene->AlertNoteMissed();
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
// Note
//--------------------------

// Public

void Note::InitSceneRef(GameplayScene* scene)
{
	Note::gameplayScene = scene;
}

void Note::BeginDrop(int x, int column)
{
	m_currentColumn = column;

	// Set sprite texture
	m_sprite->setTexture(*AssetManager::GetNoteTexture(column));

	// Elapsed time
	m_elapsedFallTime = 0.0f;

	// Get bounds
	m_width = m_sprite->getGlobalBounds().width;
	m_height = m_sprite->getGlobalBounds().height;

	// Y positions
	m_startY = 0 - m_height;
	m_targetY = Receptor::GetReceptorY();

	// Set pos
	SetPosition(x, m_startY);

	// Activate
	Activate();
}

void Note::EndDrop()
{
	m_active = false;
}

// Protected

void Note::Activate()
{
	m_active = true;
	OnActivate();
}