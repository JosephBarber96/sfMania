#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

#include "nlohmann/json.hpp"

#include "AssetManager.h"
#include "AudioManager.h"
#include "Animation.h"

using nlohmann::json;

// Texture Asset

void to_json(json& j, const TextureAsset& p) {
	j = json{ {"filepath", p.filepath}, {"texture", p.texture} };
}

void from_json(const json& j, TextureAsset& p) {
	j.at("filepath").get_to(p.filepath);
	j.at("texture").get_to(p.texture);
}

// Animation Asset

void to_json(json& j, const AnimationAsset& p) {
	j = json{ 
	{"filepath", p.filepath}, 
	{"animation", p.animation}, 
	{"frames", p.frames},
	{"width", p.width},
	{"height", p.height},
	{"xOffset", p.xOffset},
	{"yOffset", p.yOffset},
	{"frameTime", p.frameTime}
	};
}

void from_json(const json& j, AnimationAsset& p) {
	j.at("filepath").get_to(p.filepath);
	j.at("animation").get_to(p.animation);
	j.at("frames").get_to(p.frames);
	j.at("width").get_to(p.width);
	j.at("height").get_to(p.height);
	j.at("xOffset").get_to(p.xOffset);
	j.at("yOffset").get_to(p.yOffset);
	j.at("frameTime").get_to(p.frameTime);
}


// Font Asset

void to_json(json& j, const FontAsset& p) {
	j = json{ {"filepath", p.filepath}, {"font", p.font} };
}

void from_json(const json& j, FontAsset& p) {
	j.at("filepath").get_to(p.filepath);
	j.at("font").get_to(p.font);
}

// Sound Asset

void to_json(json& j, const SoundAsset& p) {
	j = json{ {"filepath", p.filepath}, {"sound", p.sound} };
}

void from_json(const json& j, SoundAsset& p) {
	j.at("filepath").get_to(p.filepath);
	j.at("sound").get_to(p.sound);
}

// AssetInfo

void to_json(json& j, const AssetInfo& p) {
	j = json{ {"textures", p.textures}, {"fonts", p.fonts}, {"sounds", p.sounds} };
}

void from_json(const json& j, AssetInfo& p) {
	j.at("textures").get_to(p.textures);
	j.at("animations").get_to(p.animations);
	j.at("fonts").get_to(p.fonts);
	j.at("sounds").get_to(p.sounds);
}

AssetManager* AssetManager::instance;

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}

void AssetManager::Init()
{
	instance = new AssetManager();

	std::ifstream i("Assets\\assetInfo.json");
	json j;
	i >> j;

	instance->m_assetInfo = j.get<AssetInfo>();
	instance->LoadFonts();
	instance->LoadSounds();
	instance->LoadSprites();
	instance->LoadAnimations();
}

sf::Font* AssetManager::GetFont(eFont font)
{
	return instance->m_fonts[font];
}

sf::SoundBuffer* AssetManager::GetSound(eSound sound)
{
	return instance->m_sounds[sound];
}

sf::Texture* AssetManager::GetTexture(eTexture texture)
{
	return instance->m_textures[texture];
}

sf::Texture* AssetManager::GetNoteTexture(int column)
{
	switch (column)
	{
	case 0:
		return GetTexture(eTexture::arrow_left);
	case 1:
		return GetTexture(eTexture::arrow_down);
	case 2:
		return GetTexture(eTexture::arrow_up);
	case 3:
		return GetTexture(eTexture::arrow_right);
	}
}

Animation* AssetManager::GetAnimation(eAnimation anim)
{
	return instance->m_animations[anim];
}


// Private

void AssetManager::LoadFonts()
{
	m_fonts = std::map<eFont, sf::Font*>();

	for (int i = 0; i < m_assetInfo.fonts.size(); i++)
	{
		eFont font = m_assetInfo.fonts[i].font;
		m_fonts.insert(std::pair<eFont, sf::Font*>(font, new sf::Font()));
		m_fonts[font]->loadFromFile(m_assetInfo.fonts[i].filepath);
	}
}

void AssetManager::LoadSounds()
{
	m_sounds = std::map<eSound, sf::SoundBuffer*>();

	for (int i = 0; i < m_assetInfo.sounds.size(); i++)
	{
		eSound sound = m_assetInfo.sounds[i].sound;
		m_sounds.insert(std::pair<eSound, sf::SoundBuffer*>(sound, new sf::SoundBuffer()));
		m_sounds[sound]->loadFromFile(m_assetInfo.sounds[i].filepath);
	}
}

void AssetManager::LoadSprites()
{
	m_textures = std::map<eTexture, sf::Texture*>();

	for (int i = 0; i < m_assetInfo.textures.size(); i++)
	{
		eTexture texture = m_assetInfo.textures[i].texture;
		m_textures.insert(std::pair<eTexture, sf::Texture*>(texture, new sf::Texture()));
		m_textures[texture]->loadFromFile(m_assetInfo.textures[i].filepath);
	}
}

void AssetManager::LoadAnimations()
{
	m_animations = std::map<eAnimation, Animation*>();

	for (int i = 0; i < m_assetInfo.animations.size(); i++)
	{
		Animation* animation = new Animation();
		animation->SetupAnimation(m_assetInfo.animations[i]);

		eAnimation anim = m_assetInfo.animations[i].animation;

		m_animations.insert(std::pair<eAnimation, Animation*>(anim, animation));
	}
}