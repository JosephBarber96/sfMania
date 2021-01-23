#include "SettingsScene.h"

#include "AssetManager.h"
#include "GameManager.h"
#include "Input.h"
#include "Settings.h"
#include "Utility.h"


SettingsScene::SettingsScene()
{
}


SettingsScene::~SettingsScene()
{
}


//--------------------------
// Scene
//--------------------------

void SettingsScene::InitScene(SceneChangeArgs* args)
{
	m_selectedOption = eOptions::resolution;
	m_selectedResolution = Settings::Resolution();

	int ttsz = 45; // Title text size
	int htsz = 30; // Header text size
	int stsz = 25; // Small text size

	// Setup text
	Utility::SetupText(&m_heading, AssetManager::GetFont(eFont::big), "Settings.", ttsz, sf::Color::White, sf::Color::Black, 3);	
	Utility::SetupText(&m_resolutionHeadingText, AssetManager::GetFont(eFont::big), "Resolution:", htsz, sf::Color::White, sf::Color::Black, 3);
	Utility::SetupText(&m_currentResolutionText, AssetManager::GetFont(eFont::small), Utility::GetStringForResolution(Settings::Resolution()), stsz, sf::Color::White, sf::Color::Black, 3);
	Utility::SetupText(&m_fullscreenText, AssetManager::GetFont(eFont::big), "Toggle Fullscreen", htsz, sf::Color::White, sf::Color::Black, 3);
	Utility::SetupText(&m_goBackText, AssetManager::GetFont(eFont::big), "Go back.", htsz, sf::Color::White, sf::Color::Black, 3);

	// Highlight
	m_highlight = sf::RectangleShape();
	m_highlight.setFillColor(Utility::HighlightedColour());

	// Position everything
	// (Done in a separate func so we can re-use behaviour for when resolution changes)
	PositionScene();
}

void SettingsScene::UnloadScene()
{

}

void SettingsScene::UpdateScene()
{
	// Navigate
	HandleNavigation();

	// Enter
	if (Input::Enter.m_keyPressed)
	{
		OnEnterPressed();
	}

	// Back
	if (Input::Esc.m_keyPressed)
	{
		GameManager::ChangeScene(eScenes::mainMenu);
	}
}

void SettingsScene::UpdateSceneTransition(float normalized)
{

}

void SettingsScene::RenderScene(sf::RenderWindow* window)
{
	window->draw(m_heading);
	window->draw(m_resolutionHeadingText);
	window->draw(m_currentResolutionText);
	window->draw(m_fullscreenText);
	window->draw(m_goBackText);

	window->draw(m_highlight);
}



//--------------------------
// Settings Scene
//--------------------------

void SettingsScene::PositionScene()
{
	// Setup text
	m_heading.setPosition(Utility::NormalizedToScreen(&m_heading, 0.5f, 0.05f));
	m_resolutionHeadingText.setPosition(Utility::NormalizedToScreen(&m_resolutionHeadingText, 0.5f, GetYForSetting(eOptions::resolution, true)));
	m_currentResolutionText.setPosition(Utility::NormalizedToScreen(&m_currentResolutionText, 0.5f, GetYForSetting(eOptions::resolution, false)));
	m_fullscreenText.setPosition(Utility::NormalizedToScreen(&m_fullscreenText, 0.5f, GetYForSetting(eOptions::fullScreen)));
	m_goBackText.setPosition(Utility::NormalizedToScreen(&m_goBackText, 0.5f, GetYForSetting(eOptions::goBack)));

	// Highlight
	m_highlight.setSize(sf::Vector2f(Settings::WindowX() * 0.4f, Settings::WindowY() * 0.05f));
	OnSelectedOptionChanged();
}

void SettingsScene::HandleNavigation()
{
	// Up / down
	if (Input::Up.m_keyPressed && m_selectedOption > eOptions::options_first + 1)
	{
		m_selectedOption--;
		OnSelectedOptionChanged();
	}
	else if (Input::Down.m_keyPressed && m_selectedOption < eOptions::options_last - 1)
	{
		m_selectedOption++;
		OnSelectedOptionChanged();
	}


	// Left / right
	if (Input::Right.m_keyPressed || Input::Left.m_keyPressed)
	{
		bool right = Input::Right.m_keyPressed;

		switch (m_selectedOption)
		{
		case eOptions::resolution:
			if (right && m_selectedResolution < eResolution::res_last - 1)
			{
				m_selectedResolution++;
				OnSelectedResolutionChanged();
			}
			else if (!right && m_selectedResolution > eResolution::res_first + 1)
			{
				m_selectedResolution--;
				OnSelectedResolutionChanged();
			}
			break;
		}
	}
}

void SettingsScene::OnEnterPressed()
{
	switch (m_selectedOption)
	{
	case eOptions::resolution:
		ApplySelectedResolution();
		break;

	case eOptions::fullScreen:
		Settings::SetFullscreen(!Settings::FullScreen());
		break;

	case eOptions::goBack:
		GameManager::ChangeScene(eScenes::mainMenu);
		break;
	}
}

void SettingsScene::OnSelectedOptionChanged()
{
	m_highlight.setPosition(
		(Settings::WindowX() * 0.5f) - (m_highlight.getGlobalBounds().width * 0.5f),
		(Settings::WindowY() * GetYForSetting((eOptions)m_selectedOption)) - (m_highlight.getGlobalBounds().height * 0.5f));
}

void SettingsScene::OnSelectedResolutionChanged()
{
	m_currentResolutionText.setString(Utility::GetStringForResolution((eResolution)m_selectedResolution));
	sf::Vector2f pos = m_resolutionHeadingText.getPosition();
	float x = Utility::GetXForText(&m_resolutionHeadingText, 0.5f);
	m_resolutionHeadingText.setPosition(x, pos.y);
}

void SettingsScene::ApplySelectedResolution()
{
	if (Settings::Resolution() == (eResolution)m_selectedResolution) { return; }

	Settings::SetWindowSize((eResolution)m_selectedResolution);
	PositionScene();
}

float SettingsScene::GetYForSetting(eOptions option, bool header)
{
	switch (option)
	{
	case eOptions::resolution:
		if (header)
			return 0.2f;
		else
			return 0.25f;

	case eOptions::fullScreen:
		return 0.4f;

	case eOptions::goBack:
		return 0.9f;
	}
}