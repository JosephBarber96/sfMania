#pragma once

#include <string>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum eFont
{
	small			= 0,
	big				= 1,
	bold			= 2,
};

enum eTexture
{
	arrow_left		= 0,
	arrow_down		= 1,
	arrow_up		= 2,
	arrow_right		= 3,
};

enum eSound
{
	menuConfirm		= 0,
	menuCancel		= 1,
	menuSelect		= 2,
	sceneTransition	= 3,
	scroll			= 4,
};

struct TextureAsset
{
	std::string filepath;
	eTexture texture;
};

struct FontAsset 
{
	std::string filepath;
	eFont font;
};

struct SoundAsset
{
	std::string filepath;
	eSound sound;
};

struct AssetInfo
{
	std::vector<TextureAsset> textures;
	std::vector<FontAsset> fonts;
	std::vector<SoundAsset> sounds;
};

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	static void Init();

	static std::string AssetPath() { return "Assets\\"; }

	static sf::Font* GetFont(eFont font);
	static sf::SoundBuffer* GetSound(eSound sound);
	static sf::Texture* GetTexture(eTexture texture);
	static sf::Texture* GetNoteTexture(int column);

private:

	static void LoadFonts();
	static void LoadSounds();
	static void LoadSprites();

	static AssetManager* instance;

	// Maps
	std::map<eFont, sf::Font*> m_fonts;
	std::map<eSound, sf::SoundBuffer*> m_sounds;
	std::map<eTexture, sf::Texture*> m_textures;

	// Asset info 
	AssetInfo m_assetInfo;
};

