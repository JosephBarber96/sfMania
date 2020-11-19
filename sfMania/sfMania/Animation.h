#pragma once

#include "GameObject.h"

class AnimationAsset;

class Animation : public GameObject
{
public:
	Animation();
	~Animation();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow* window);
	void OnSetPosition();

	// Animation
	void SetupAnimation(AnimationAsset asset);
	void Play();
	void SetSize(int width, int height);
	void SetColour(sf::Color c);

private:
	// Value vars
	bool m_isPlaying;
	int m_currentFrame;
	sf::IntRect m_currentTextureRect;
	float m_elapsedFrameTime;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	// Animation info 
	int m_frames;
	int m_width;
	int m_height;
	int m_xOffset;
	int m_yOffset;
	float m_frameTime;

	void AdvanceFrame();
};