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
	void UpdateSceneTransition(float normalized);
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
	sf::Sprite* m_bgSprite;

	// Title
	sf::Text m_Title;

	// Text options
	sf::Text m_Start;
	sf::Text m_Options;
	sf::Text m_Quit;

	// Arrows
	sf::Sprite* m_menuArrowRightSprite;
	sf::Sprite* m_menuArrowLeftSprite;

	void MenuUp();
	void MenuDown();
	void OnMenuChange();
	float GetYForOption(int index);
};