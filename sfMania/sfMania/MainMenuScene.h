#pragma once

#include "Scene.h"

class MediaBox;

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	~MainMenuScene();

	void InitScene();
	void UnloadScene();

	void UpdateScene();
	void RenderScene(sf::RenderWindow * window);

private:

	enum eMainMenuOptions : int
	{
		first			=	0,
		option_start	=	1,
		option_options	=	2,
		option_quit		=	3,
		last			=	4
	};

	int m_currentOption;

	// Menu Background
	sf::Texture* m_bgTexture;
	sf::Sprite* m_bgSprite;

	// Text options
	sf::Text m_Start;
	sf::Text m_Options;
	sf::Text m_Quit;

	// Arrow
	sf::Texture* m_menuArrowRightTexture;
	sf::Sprite* m_menuArrowRightSprite;
	sf::Texture* m_menuArrowLeftTexture;
	sf::Sprite* m_menuArrowLeftSprite;

	void MenuUp();
	void MenuDown();
	void OnMenuChange();
	float GetYForOption(int index);
};