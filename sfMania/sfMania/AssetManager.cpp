#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream

#include "nlohmann/json.hpp"

#include "AssetManager.h"
#include "AudioManager.h"

using nlohmann::json;

// Texture Asset

void to_json(json& j, const TextureAsset& p) {
	j = json{ {"filepath", p.filepath}, {"texture", p.texture} };
}

void from_json(const json& j, TextureAsset& p) {
	j.at("filepath").get_to(p.filepath);
	j.at("texture").get_to(p.texture);
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

	LoadFonts();
	LoadSounds();
	LoadSprites();
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


// Private

void AssetManager::LoadFonts()
{
	instance->m_fonts = std::map<eFont, sf::Font*>();

	for (int i = 0; i < instance->m_assetInfo.fonts.size(); i++)
	{
		eFont font = instance->m_assetInfo.fonts[i].font;
		instance->m_fonts.insert(std::pair<eFont, sf::Font*>(font, new sf::Font()));
		instance->m_fonts[font]->loadFromFile(instance->m_assetInfo.fonts[i].filepath);
	}
}

void AssetManager::LoadSounds()
{
	instance->m_sounds = std::map<eSound, sf::SoundBuffer*>();

	for (int i = 0; i < instance->m_assetInfo.sounds.size(); i++)
	{
		eSound sound = instance->m_assetInfo.sounds[i].sound;
		instance->m_sounds.insert(std::pair<eSound, sf::SoundBuffer*>(sound, new sf::SoundBuffer()));
		instance->m_sounds[sound]->loadFromFile(instance->m_assetInfo.sounds[i].filepath);
	}
}

void AssetManager::LoadSprites()
{
	instance->m_textures = std::map<eTexture, sf::Texture*>();

	for (int i = 0; i < instance->m_assetInfo.textures.size(); i++)
	{
		eTexture texture = instance->m_assetInfo.textures[i].texture;
		instance->m_textures.insert(std::pair<eTexture, sf::Texture*>(texture, new sf::Texture()));
		instance->m_textures[texture]->loadFromFile(instance->m_assetInfo.textures[i].filepath);
	}
}