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
	switch (column)
	{
		// Left
	case 0:
		m_textureDown = AssetManager::GetTexture(eTexture::receptor_left_pressed);
		m_textureUp = AssetManager::GetTexture(eTexture::receptor_left);
		break;

		// Down
	case 1:
		m_textureDown = AssetManager::GetTexture(eTexture::receptor_down_pressed);
		m_textureUp = AssetManager::GetTexture(eTexture::receptor_down);
		break;

		// Up
	case 2:
		m_textureDown = AssetManager::GetTexture(eTexture::receptor_up_pressed);
		m_textureUp = AssetManager::GetTexture(eTexture::receptor_up);
		break;

		// Right
	case 3:
		m_textureDown = AssetManager::GetTexture(eTexture::receptor_right_pressed);
		m_textureUp = AssetManager::GetTexture(eTexture::receptor_right);
		break;
	}

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