#pragma once

#include "Scene.h"

class Score;

class ResultsScene : public Scene
{
public:
	ResultsScene();
	~ResultsScene();

	// Scene
	void InitScene();
	void UnloadScene();
	void UpdateScene();
	void RenderScene(sf::RenderWindow * window);

	// Results Scene
	static void LoadSceneResults(Score* score);

private:
	static Score* score;

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