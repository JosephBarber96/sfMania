#pragma once

#include <SFML/Audio.hpp>

enum eSounds
{
	menuSelect,
	menuCancel,
	sceneTransition
};

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static void Init();
	static void PlaySound(eSounds sound);

private:
	static AudioManager* instance;

	sf::Sound sound;
};

