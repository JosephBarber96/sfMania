#pragma once

#include "MediaBox.h"

class StepMap;

class BeatmapDifficultyPanel : public MediaBox
{
public:
	BeatmapDifficultyPanel();
	~BeatmapDifficultyPanel();

	// GameObject
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	virtual void OnMediaBoxSetPosition();

	// Song difficulty panel
	void SetInfo(StepMap* beatmap);
	void Highlight();
	void UnHighlight();

protected:
	sf::Text* m_difficultyText;
};