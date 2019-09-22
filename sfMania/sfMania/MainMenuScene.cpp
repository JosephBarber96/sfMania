#include <stdlib.h>

#include "MainMenuScene.h"
#include "Input.h"
#include "GameManager.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "Settings.h"

MainMenuScene::MainMenuScene() {}

MainMenuScene::~MainMenuScene() {}

void MainMenuScene::InitScene()
{
	// Menu background
	m_bgTexture = new sf::Texture();
	m_bgTexture->loadFromFile(ResourceManager::AssetPath() + "Sprites\\menuBackground.png");
	m_bgSprite = new sf::Sprite();
	m_bgSprite->setTexture(*m_bgTexture);
	m_bgSprite->setScale(Utility::GetScaleForTargetSize(m_bgTexture, Settings::WindowX(), Settings::WindowY()));

	// Text
	int fontSize = 35;

	m_Start = sf::Text();
	Utility::SetupText(&m_Start, ResourceManager::GetFont(eFont::bold), "Start", fontSize, sf::Color::White, sf::Color::Black, 2);
	m_Start.setPosition(Settings::WindowX() * 0.5f - m_Start.getGlobalBounds().width / 2, GetYForOption(eMainMenuOptions::option_start));

	m_Options = sf::Text();
	Utility::SetupText(&m_Options, ResourceManager::GetFont(eFont::bold), "Options", fontSize, sf::Color::White, sf::Color::Black, 2);
	m_Options.setPosition(Settings::WindowX() * 0.5f - m_Options.getGlobalBounds().width / 2, GetYForOption(eMainMenuOptions::option_options));

	m_Quit = sf::Text();
	Utility::SetupText(&m_Quit, ResourceManager::GetFont(eFont::bold), "Quit", fontSize, sf::Color::White, sf::Color::Black, 2);
	m_Quit.setPosition(Settings::WindowX() * 0.5f - m_Quit.getGlobalBounds().width / 2, GetYForOption(eMainMenuOptions::option_quit));

	// Menu arrow
	m_menuArrowRightTexture = new sf::Texture();
	m_menuArrowRightTexture->loadFromFile(ResourceManager::AssetPath() + "Sprites\\MenuArrowRight.png");
	m_menuArrowRightSprite = new sf::Sprite();
	m_menuArrowRightSprite->setTexture(*m_menuArrowRightTexture);
	m_menuArrowRightSprite->setPosition(Settings::WindowX() * 0.3f, GetYForOption(eMainMenuOptions::option_start));
	m_menuArrowRightSprite->setScale(0.75f, 0.75f);

	m_menuArrowLeftTexture = new sf::Texture();
	m_menuArrowLeftTexture->loadFromFile(ResourceManager::AssetPath() + "Sprites\\MenuArrowLeft.png");
	m_menuArrowLeftSprite = new sf::Sprite();
	m_menuArrowLeftSprite->setTexture(*m_menuArrowLeftTexture);
	m_menuArrowLeftSprite->setPosition(Settings::WindowX() * 0.7f - m_menuArrowLeftSprite->getGlobalBounds().width, GetYForOption(eMainMenuOptions::option_start));
	m_menuArrowLeftSprite->setScale(0.75f, 0.75f);

	m_currentOption = eMainMenuOptions::option_start;
	OnMenuChange();
}

void MainMenuScene::UnloadScene()
{
	delete m_bgTexture;
	delete m_bgSprite;

	delete m_menuArrowRightTexture;
	delete m_menuArrowRightSprite;
	delete m_menuArrowLeftTexture;
	delete m_menuArrowLeftSprite;
}



void MainMenuScene::UpdateScene()
{
	if (Input::Enter.m_keyPressed)
	{
		switch (m_currentOption)
		{
		case eMainMenuOptions::option_start:
			GameManager::ChangeScene(eScenes::songSelect);
			break;

		case eMainMenuOptions::option_options:
			GameManager::ChangeScene(eScenes::options);
			break;

		case eMainMenuOptions::option_quit:
			exit(EXIT_SUCCESS);
			break;
		}
	}

	if (Input::Up.m_keyPressed)
	{
		MenuUp();
	}
	if (Input::Down.m_keyPressed)
	{
		MenuDown();
	}
}

void MainMenuScene::RenderScene(sf::RenderWindow * window)
{
	window->draw(*m_bgSprite);

	window->draw(m_Start);
	window->draw(m_Options);
	window->draw(m_Quit);

	window->draw(*m_menuArrowRightSprite);
	window->draw(*m_menuArrowLeftSprite);
}



//--------------------------
// MainMenuScene
//--------------------------

// Private

void MainMenuScene::MenuUp()
{
	if (m_currentOption > eMainMenuOptions::first + 1)
	{
		m_currentOption--;
		OnMenuChange();
	}
}

void MainMenuScene::MenuDown()
{
	if (m_currentOption < eMainMenuOptions::last - 1)
	{
		m_currentOption++;
		OnMenuChange();
	}
}

void MainMenuScene::OnMenuChange()
{
	m_menuArrowRightSprite->setPosition(m_menuArrowRightSprite->getPosition().x, GetYForOption(m_currentOption));
	m_menuArrowLeftSprite->setPosition(m_menuArrowLeftSprite->getPosition().x, GetYForOption(m_currentOption));
}

float MainMenuScene::GetYForOption(int index)
{
	switch (index)
	{
	case eMainMenuOptions::option_start:
		return Settings::WindowY() * 0.5f;
	case eMainMenuOptions::option_options:
		return Settings::WindowY() * 0.65f;
	case eMainMenuOptions::option_quit:
		return Settings::WindowY() * 0.8f;
	}

	return 0;
}