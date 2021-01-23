#include <stdlib.h>

#include "MainMenuScene.h"
#include "Input.h"
#include "GameManager.h"
#include "AssetManager.h"
#include "Utility.h"
#include "Settings.h"
#include "AudioManager.h"

MainMenuScene::MainMenuScene()
{
	m_bgSprite = new sf::Sprite();
	m_menuArrowRightSprite = new sf::Sprite();
	m_menuArrowLeftSprite = new sf::Sprite();
}


MainMenuScene::~MainMenuScene()
{
	delete m_bgSprite;
	delete m_menuArrowRightSprite;
	delete m_menuArrowLeftSprite;
}

void MainMenuScene::InitScene(SceneChangeArgs* args)
{
	// Menu background

	m_bgSprite->setTexture(*AssetManager::GetTexture(eTexture::main_menu_bg));
	m_bgSprite->setScale(Utility::GetScaleForTargetSize(AssetManager::GetTexture(eTexture::main_menu_bg), Settings::WindowX(), Settings::WindowY()));

	int titleFontSize = 72;
	m_Title = sf::Text();
	Utility::SetupText(&m_Title, AssetManager::GetFont(eFont::bold), "SFMania", titleFontSize, sf::Color::White, sf::Color::Black, 2);
	m_Title.setPosition(Utility::NormalizedToScreen(&m_Title, 0.5f, 0.25f));

	// Text
	int fontSize = 35;

	m_Start = sf::Text();
	Utility::SetupText(&m_Start, AssetManager::GetFont(eFont::bold), "Start", fontSize, sf::Color::White, sf::Color::Black, 2);
	m_Start.setPosition(Settings::WindowX() * 0.5f - m_Start.getGlobalBounds().width / 2, GetYForOption(eMainMenuOptions::option_start));

	m_Options = sf::Text();
	Utility::SetupText(&m_Options, AssetManager::GetFont(eFont::bold), "Options", fontSize, sf::Color::White, sf::Color::Black, 2);
	m_Options.setPosition(Settings::WindowX() * 0.5f - m_Options.getGlobalBounds().width / 2, GetYForOption(eMainMenuOptions::option_options));

	m_Quit = sf::Text();
	Utility::SetupText(&m_Quit, AssetManager::GetFont(eFont::bold), "Quit", fontSize, sf::Color::White, sf::Color::Black, 2);
	m_Quit.setPosition(Settings::WindowX() * 0.5f - m_Quit.getGlobalBounds().width / 2, GetYForOption(eMainMenuOptions::option_quit));

	// Menu arrows
	m_menuArrowRightSprite->setTexture(*AssetManager::GetTexture(eTexture::main_menu_arrow_right));
	m_menuArrowRightSprite->setPosition(Settings::WindowX() * 0.3f, GetYForOption(eMainMenuOptions::option_start));
	m_menuArrowRightSprite->setScale(0.75f, 0.75f);

	m_menuArrowLeftSprite->setTexture(*AssetManager::GetTexture(eTexture::main_menu_arrow_left));
	m_menuArrowLeftSprite->setPosition(Settings::WindowX() * 0.7f - m_menuArrowLeftSprite->getGlobalBounds().width, GetYForOption(eMainMenuOptions::option_start));
	m_menuArrowLeftSprite->setScale(0.75f, 0.75f);

	m_currentOption = eMainMenuOptions::option_start;
	OnMenuChange();
}

void MainMenuScene::UnloadScene()
{
	// todo, cleanup
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

		AudioManager::PlaySound(eSound::menuConfirm);
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

void MainMenuScene::UpdateSceneTransition(float normalized)
{

}

void MainMenuScene::RenderScene(sf::RenderWindow * window)
{
	window->draw(*m_bgSprite);

	window->draw(m_Title);

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
	AudioManager::PlaySound(eSound::menuSelect);
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