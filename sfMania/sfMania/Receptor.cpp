#include <string>

#include "Receptor.h"

#include "GameManager.h"
#include "Settings.h"
#include "Maths.h"
#include "AssetManager.h"

Receptor::Receptor()
{
	m_textureDown = new sf::Texture();
	m_textureUp = new sf::Texture();
	m_sprite = new sf::Sprite();
}


Receptor::~Receptor()
{
	delete m_textureUp;
	delete m_textureDown;
	delete m_sprite;
}




//--------------------------
// GameObject
//--------------------------

void Receptor::Update()
{
	if (m_pushDown)
	{
		m_elapsedPushTime += GameManager::DeltaTime();
		float normalized = m_elapsedPushTime / 0.15f;
		float scale = Maths::Lerp(0.85f, 1.0f, normalized);
		scale = Maths::Clamp(scale, 0, 1);
		m_sprite->setScale(scale, scale);

		if (normalized >= 1)
			m_pushDown = false;
	}
}

void Receptor::RenderSelf(sf::RenderWindow * window)
{
	window->draw(*m_sprite);
}


void Receptor::OnSetPosition()
{
	m_sprite->setPosition(
		m_x + m_sprite->getGlobalBounds().width,
		m_y + m_sprite->getGlobalBounds().height);
}




//--------------------------
// Receptor
//--------------------------

void Receptor::InitSelf(int column, int xPos)
{
	// Set pos
	SetPosition(xPos, GetReceptorY());

	// Setup textures
	std::string pressed = AssetManager::AssetPath() + "Sprites\\";
	std::string unpressed = AssetManager::AssetPath() + "Sprites\\";
	switch (column)
	{
		// Left
	case 0:
		pressed.append("Receptor_Left_Pressed.png");
		unpressed.append("Receptor_Left.png");
		break;

		// Down
	case 1:
		pressed.append("Receptor_Down_Pressed.png");
		unpressed.append("Receptor_Down.png");
		break;

		// Up
	case 2:
		pressed.append("Receptor_Up_Pressed.png");
		unpressed.append("Receptor_Up.png");
		break;

		// Right
	case 3:
		pressed.append("Receptor_Right_Pressed.png");
		unpressed.append("Receptor_Right.png");
		break;
	}

	// Load textures
	m_textureDown->loadFromFile(pressed);
	m_textureUp->loadFromFile(unpressed);

	// Set sprite
	m_sprite->setTexture(*m_textureUp);
	m_sprite->setOrigin(0, 0);
}

void Receptor::Pressed()
{
	m_sprite->setTexture(*m_textureDown);
	m_pressed = true;
	m_pushDown = true;
	m_elapsedPushTime = 0.0f;
}

void Receptor::Released()
{
	m_sprite->setTexture(*m_textureUp);
	m_pressed = false;
}

// Static

float Receptor::GetReceptorY()
{
	return Settings::WindowY() * 0.8f;
}