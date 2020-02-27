#pragma once

#include <SFML/Audio.hpp>

class Song;

enum eSounds
{
	menuConfirm,
	menuCancel,
	menuSelect,
	sceneTransition,
	scroll,
};

class AudioManager
{
public:
	static void Init();
	static void PlaySound(eSounds sound);
	static void PlayMusic(Song* song);
	static void StopMusic();

private:
	AudioManager();
	~AudioManager();

	static AudioManager* instance;

	sf::Sound m_sound;
	sf::Music* m_music;
};

