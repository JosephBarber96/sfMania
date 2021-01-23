#pragma once

#include "Scene.h"

class Score;

class ResultsScene : public Scene
{
public:
	ResultsScene();
	~ResultsScene();

	// Scene
	void InitScene(SceneChangeArgs* args);
	void UnloadScene();
	void UpdateScene();
	void UpdateSceneTransition(float normalized);
	void RenderScene(sf::RenderWindow * window);

private:
	sf::Texture *m_backgroundTexture;
	sf::Sprite *m_backgroundSprite;

	sf::Text m_scoreText;

	sf::Text m_maxComboTitle;
	sf::Text m_perfectNotesTitle;
	sf::Text m_greatNotesTitle;
	sf::Text m_missNotesTitle;

	sf::Text m_maxCombo;
	sf::Text m_perfectNotes;
	sf::Text m_greatNotes;
	sf::Text m_missNotes;

	sf::RectangleShape m_scoreBox;
};