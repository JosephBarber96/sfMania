#include <string>

#include "Receptor.h"

#include "GameManager.h"
#include "Settings.h"
#include "Maths.h"
#include "AssetManager.h"
#include "Animation.h"

Receptor::Receptor()
{
	m_textureDown = new sf::Texture();
	m_textureUp = new sf::Texture();
	m_sprite = new sf::Sprite();
	m_hitAnim = new Animation();
}


Receptor::~Receptor()
{
	delete m_textureDown;
	delete m_textureUp;
	delete m_sprite;
	delete m_hitAnim;
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
	m_hitAnim->RenderSelf(window);
}


void Receptor::OnSetPosition()
{
	m_sprite->setPosition(m_x, m_y);

	int sz = m_sprite->getGlobalBounds().width;
	m_hitAnim->SetSize(sz * 2, sz * 2);
	m_hitAnim->SetPosition(m_x - sz * 0.5f, m_y - sz * 0.5f);
	
}




//--------------------------
// Receptor
//--------------------------

void Receptor::InitSelf(int column, int xPos)
{
	// Setup anim
	m_hitAnim->SetupAnimation(AssetManager::GetAnimation(eAnimation::receptor_hit));

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

	// Set pos
	SetPosition(xPos, GetReceptorY());
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

void Receptor::NoteHit()
{
	m_hitAnim->Play();
}

// Static

float Receptor::GetReceptorY()
{
	return Settings::WindowY() * 0.8f;
}