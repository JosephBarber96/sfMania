#pragma once

#include "MediaBox.h"

class DifficultyPanel : public MediaBox
{
public:
	DifficultyPanel();
	~DifficultyPanel();

	// GameObject
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	void OnMediaBoxSetPosition();

	// DifficultyPanel
	void SetChosen(bool chosen);
	void SetDifficultyText(std::string str);

private:
	sf::Text m_difficultyText;

	void PositionText();
};
