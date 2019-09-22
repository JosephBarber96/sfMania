#include "BeatmapDifficultyPanel.h"

#include "StepMap.h"
#include "ResourceManager.h"
#include "Utility.h"


BeatmapDifficultyPanel::BeatmapDifficultyPanel()
{
	m_difficultyText = new sf::Text();
	Utility::SetupText(m_difficultyText, eFont::small, "", 25, sf::Color::White, sf::Color::Black, 2, 5, 5);
}

BeatmapDifficultyPanel::~BeatmapDifficultyPanel()
{
	delete m_difficultyText;
}




//--------------------------
// GameObject
//--------------------------

void BeatmapDifficultyPanel::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(*m_difficultyText);
}




//--------------------------
// MediaBox
//--------------------------

void BeatmapDifficultyPanel::OnMediaBoxSetPosition()
{
	m_difficultyText->setPosition(m_x + 5, m_y + 5);
}




//--------------------------
// BeatmapDifficultyPanel
//--------------------------

void BeatmapDifficultyPanel::SetInfo(StepMap* stepmap)
{
	std::string difficulty = "";
	difficulty.append(std::to_string(stepmap->m_numericalDifficulty));
	difficulty.append(" ");
	difficulty.append(stepmap->m_difficultyName);

	m_difficultyText->setString(difficulty);
}

void BeatmapDifficultyPanel::Highlight()
{
	MediaBox::SetFillColour(Utility::HighlightedColour());
}

void BeatmapDifficultyPanel::UnHighlight()
{
	MediaBox::SetFillColour(Utility::UnhighlightedColour());
}