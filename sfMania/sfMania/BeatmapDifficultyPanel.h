#pragma once

#include "MediaBox.h"

class StepMap;

class BeatmapDifficultyPanel : public MediaBox
{
public:

	const int TEXT_SIZE = 35;

	BeatmapDifficultyPanel();
	~BeatmapDifficultyPanel();

	// GameObject
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	virtual void OnMediaBoxSetPosition();

	// Song difficulty panel
	void SetInfo(StepMap* beatmap);
	void SetDifficultyColour(sf::Color col);
	void Highlight();
	void UnHighlight();

protected:
	sf::Text m_difficultyText;
	sf::Color m_colour;

	void PositionText();
};