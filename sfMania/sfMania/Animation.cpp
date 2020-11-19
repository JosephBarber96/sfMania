#include "Animation.h"
#include "GameManager.h"
#include "AssetManager.h"


Animation::Animation()
{
	m_texture = sf::Texture();
	m_sprite = sf::Sprite();
}

Animation::~Animation()
{
	
}

void Animation::Update()
{
	m_elapsedFrameTime += GameManager::DeltaTime();

	if (m_elapsedFrameTime > m_frameTime)
	{
		m_elapsedFrameTime -= m_frameTime;
		AdvanceFrame();
	}
}

void Animation::RenderSelf(sf::RenderWindow* window)
{
	window->draw(m_sprite);
}

void Animation::OnSetPosition()
{
	m_sprite.setPosition(m_x, m_y);
}

void Animation::SetupAnimation(AnimationAsset asset)
{
	m_frames = asset.frames;
	m_width = asset.width;
	m_height = asset.height;
	m_xOffset = asset.xOffset;
	m_yOffset = asset.yOffset;
	m_frameTime = asset.frameTime;

	m_texture.loadFromFile(asset.filepath);
	m_sprite.setTexture(m_texture);
	m_currentTextureRect = sf::IntRect(0, 0, m_width, m_height); //x, y, w, h
	m_sprite.setTextureRect(m_currentTextureRect);
}

void Animation::Play()
{

}

void Animation::SetSize(int newWidth, int newHeight)
{
	float xScale = (float)newWidth / (float)m_width;
	float yScale = (float)newHeight / (float)m_height;

	m_sprite.setScale(xScale, yScale);
}

void Animation::SetColour(sf::Color c)
{
	m_sprite.setColor(c);
}

void Animation::AdvanceFrame()
{
	if (m_currentFrame < m_frames - 1)
	{
		m_currentFrame++;
		m_currentTextureRect.left += m_xOffset;
		m_currentTextureRect.top += m_yOffset;
	}
	else
	{
		m_currentFrame = 0;
		m_currentTextureRect.left = 0;
		m_currentTextureRect.top = 0;
	}

	m_sprite.setTextureRect(m_currentTextureRect);
}