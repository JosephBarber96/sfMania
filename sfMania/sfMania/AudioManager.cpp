#include "AudioManager.h"
#include "ResourceManager.h"

AudioManager* AudioManager::instance;

AudioManager::AudioManager() { }

AudioManager::~AudioManager() { }

void AudioManager::Init()
{
	instance = new AudioManager();
	instance->sound = sf::Sound();
}

void AudioManager::PlaySound(eSounds sound)
{
	instance->sound.setBuffer(*ResourceManager::GetSound(sound));
	instance->sound.play();
}