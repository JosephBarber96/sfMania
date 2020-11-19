#pragma once

#include <string>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum eFont
{
	small = 0,
	big = 1,
	bold = 2,
};

enum eTexture
{
	arrow_left = 0,
	arrow_down = 1,
	arrow_up = 2,
	arrow_right = 3,

	receptor_left_pressed = 10,
	receptor_left = 11,
	receptor_down_pressed = 12,
	receptor_down = 13,
	receptor_up_pressed = 14,
	receptor_up = 15,
	receptor_right_pressed = 16,
	receptor_right = 17,

	main_menu_bg = 100,
	main_menu_arrow_left = 101,
	main_menu_arrow_right = 102
};

enum eAnimation
{
	receptor_hit = 0,
	health_bar = 10,
};

enum eSound
{
	menuConfirm = 0,
	menuCancel = 1,
	menuSelect = 2,
	sceneTransition = 3,
	scroll = 4,
};

struct TextureAsset
{
	std::string filepath;
	eTexture texture;
};

struct AnimationAsset
{
	std::string filepath;
	eAnimation animation;
	int frames;
	int width;
	int height;
	int xOffset;
	int yOffset;
	float frameTime;
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
	std::vector<AnimationAsset> animations;
	std::vector<FontAsset> fonts;
	std::vector<SoundAsset> sounds;
};


class Animation;

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	static void Init();

	static sf::Font* GetFont(eFont font);
	static sf::SoundBuffer* GetSound(eSound sound);
	static sf::Texture* GetTexture(eTexture texture);
	static sf::Texture* GetNoteTexture(int column);
	static AnimationAsset* GetAnimation(eAnimation anim);

private:

	void LoadFonts();
	void LoadSounds();
	void LoadSprites();

	static AssetManager* instance;

	// Maps
	std::map<eFont, sf::Font*> m_fonts;
	std::map<eSound, sf::SoundBuffer*> m_sounds;
	std::map<eTexture, sf::Texture*> m_textures;
	std::map<eAnimation, AnimationAsset> m_animations;

	// Asset info 
	AssetInfo m_assetInfo;
};