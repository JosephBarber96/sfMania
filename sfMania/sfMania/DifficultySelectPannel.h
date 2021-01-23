#pragma once

#include <vector>
#include <string>

#include "MediaBox.h"
#include "Utility.h"

class Song;
class DifficultyPanel;

class DifficultySelectMenu : public MediaBox
{
public:
	DifficultySelectMenu();
	~DifficultySelectMenu();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	void OnMediaBoxSetPosition();

	// DifficultySelectPannel
	int ChosenDifficulty() const { return m_currentDifficulty; }
	void Init(Song* song);

private:

	// Renderables
	sf::RectangleShape m_menuBox;
	std::vector<DifficultyPanel*> m_difficulties;

	// Logic
	int m_currentDifficulty;
	Song* m_song;

	void UpdateHighlights();
};

