#pragma once

#include "Scene.h"

class SettingsScene : public Scene
{
public:
	SettingsScene();
	~SettingsScene();

	void InitScene();
	void UnloadScene();

	void UpdateScene();
	void UpdateSceneTransition(float normalized);
	void RenderScene(sf::RenderWindow * window);

private:

	// SFML
	sf::Text m_heading;

	sf::Text m_resolutionHeadingText;
	sf::Text m_currentResolutionText;
	sf::Text m_fullscreenText;
	sf::Text m_goBackText;

	sf::RectangleShape m_highlight;

	// OPTIONS

	enum eOptions : int
	{
		options_first	= 0,
		resolution		= 1,
		fullScreen		= 2,
		goBack			= 3,
		options_last	= 4,
	};

	int m_selectedOption;
	int m_selectedResolution;


	// METHODS

	// Init and on res changed
	void PositionScene();

	// Navigation
	void HandleNavigation();
	void OnEnterPressed();

	// Options changed
	void OnSelectedOptionChanged();
	void OnSelectedResolutionChanged();

	// Applying settings
	void ApplySelectedResolution();

	// Data
	float GetYForSetting(eOptions option, bool header = false);
};