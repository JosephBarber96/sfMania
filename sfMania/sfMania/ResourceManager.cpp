#include "ResourceManager.h"

ResourceManager* ResourceManager::instance;

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::Init()
{
	instance = new ResourceManager();

	// Font
	instance->m_fonts = std::map<eFont, sf::Font*>();
	instance->m_fonts.insert(std::pair<eFont, sf::Font*>(eFont::small, new sf::Font()));
	instance->m_fonts[eFont::small]->loadFromFile(AssetPath() + "Fonts\\editundo.ttf");
	instance->m_fonts.insert(std::pair<eFont, sf::Font*>(eFont::big, new sf::Font()));
	instance->m_fonts[eFont::big]->loadFromFile(AssetPath() + "Fonts\\VCR_OSD_MONO_1.001.ttf");
	instance->m_fonts.insert(std::pair<eFont, sf::Font*>(eFont::bold, new sf::Font()));
	instance->m_fonts[eFont::bold]->loadFromFile(AssetPath() + "Fonts\\UpheavalPro.ttf");

	// Notes
	instance->m_noteLeft = new sf::Texture();
	instance->m_noteLeft->loadFromFile(AssetPath() + "Sprites\\Arrow_Left.png");
	instance->m_noteDown = new sf::Texture();
	instance->m_noteDown->loadFromFile(AssetPath() + "Sprites\\Arrow_Down.png");
	instance->m_noteUp = new sf::Texture();
	instance->m_noteUp->loadFromFile(AssetPath() + "Sprites\\Arrow_Up.png");
	instance->m_noteRight = new sf::Texture();
	instance->m_noteRight->loadFromFile(AssetPath() + "Sprites\\Arrow_Right.png");
}


sf::Font* ResourceManager::GetFont(eFont font)
{
	return instance->m_fonts[font];
}


sf::Texture* ResourceManager::GetNoteTexture(int column)
{
	switch (column)
	{
	case 0:
		return instance->m_noteLeft;
	case 1:
		return instance->m_noteDown;
	case 2:
		return instance->m_noteUp;
	case 3:
		return instance->m_noteRight;
	}
}