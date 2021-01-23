#include "BaseNote.h"
#include "Settings.h"
#include "AssetManager.h"
#include "Receptor.h"

GameplayScene* BaseNote::gameplayScene;

BaseNote::BaseNote()
{
	m_sprite = new sf::Sprite();
	m_elapsedFallTime = 0.0f;
	m_targetFallTime = Settings::FallTime();
}

BaseNote::~BaseNote()
{
	delete m_sprite;
}




//--------------------------
// BaseNote
//--------------------------

// Public

void BaseNote::InitSceneRef(GameplayScene* scene)
{
	BaseNote::gameplayScene = scene;
}

void BaseNote::BeginDrop(int x, int column)
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

void BaseNote::EndNode()
{
	m_active = false;
}

void BaseNote::Activate()
{
	m_active = true;
}