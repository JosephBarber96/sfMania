#include <iostream>

#include "AudioManager.h"
#include "AssetManager.h"
#include "Song.h"

AudioManager* AudioManager::instance;

AudioManager::AudioManager() { }

AudioManager::~AudioManager()
{
	delete m_music;
}

void AudioManager::Init()
{
	instance = new AudioManager();
	instance->m_sound = sf::Sound();
	instance->m_music = new sf::Music();

	instance->m_sound.setVolume(50);
	instance->m_music->setVolume(50);
}

void AudioManager::PlaySound(eSound sound)
{
	instance->m_sound.setBuffer(*AssetManager::GetSound(sound));
	instance->m_sound.play();
}

void AudioManager::PlayMusic(Song* song)
{
	if (!instance->m_music->openFromFile(song->m_musicFile))
	{
		std::cout << "ERROR: Audio Manager cannot load song from file.";
		return;
	}

	instance->m_music->play();
}

void AudioManager::StopMusic()
{
	instance->m_music->stop();
}