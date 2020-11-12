#pragma once

#include <SFML/Audio.hpp>

class Song;

enum eSound;

class AudioManager
{
public:
	static void Init();
	static void PlaySound(eSound sound);
	static void PlayMusic(Song* song);
	static void StopMusic();

private:
	AudioManager();
	~AudioManager();

	static AudioManager* instance;

	sf::Sound m_sound;
	sf::Music* m_music;
};

