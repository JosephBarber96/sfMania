#pragma once

#include <string>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum eFont
{
	small,
	big,
	bold
};

enum eSounds;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static void Init();

	static std::string AssetPath() { return "Assets\\"; }

	static sf::Font* GetFont(eFont font);
	static sf::SoundBuffer* GetSound(eSounds sound);
	static sf::Texture* GetNoteTexture(int column);

private:
	static ResourceManager* instance;

	// Fonts
	std::map<eFont, sf::Font*> m_fonts;

	// Sounds
	std::map<eSounds, sf::SoundBuffer*> m_sounds;

	// Notes
	sf::Texture* m_noteLeft;
	sf::Texture* m_noteDown;
	sf::Texture* m_noteUp;
	sf::Texture* m_noteRight;
};

