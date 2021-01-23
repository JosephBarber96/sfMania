#include <string>

#include "BeatmapDifficultyPanel.h"

#include "StepMap.h"
#include "AssetManager.h"
#include "Utility.h"


BeatmapDifficultyPanel::BeatmapDifficultyPanel()
{
	m_difficultyText = sf::Text();
	Utility::SetupText(&m_difficultyText, eFont::small, "", TEXT_SIZE, sf::Color::White, sf::Color::Black, 2, 5, 5);
}

BeatmapDifficultyPanel::~BeatmapDifficultyPanel()
{

}




//--------------------------
// GameObject
//--------------------------

void BeatmapDifficultyPanel::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(m_difficultyText);
}




//--------------------------
// MediaBox
//--------------------------

void BeatmapDifficultyPanel::OnMediaBoxSetPosition()
{
	PositionText();
}




//--------------------------
// BeatmapDifficultyPanel
//--------------------------

void BeatmapDifficultyPanel::SetInfo(StepMap* stepmap)
{
	m_difficultyText.setString(std::to_string(stepmap->m_numericalDifficulty));
	PositionText();
}

void BeatmapDifficultyPanel::SetDifficultyColour(sf::Color col)
{
	m_colour = col;
}

void BeatmapDifficultyPanel::Highlight()
{
	m_colour.a = 255;
	MediaBox::SetFillColour(m_colour);
}

void BeatmapDifficultyPanel::UnHighlight()
{
	m_colour.a = 128;
	MediaBox::SetFillColour(m_colour);
}

void BeatmapDifficultyPanel::PositionText()
{
	int x = m_x + (m_box.getGlobalBounds().width * 0.5f) - (m_difficultyText.getGlobalBounds().width * 0.5f);
	int y = m_y + (m_box.getGlobalBounds().height * 0.5f) - (m_difficultyText.getGlobalBounds().height);
	m_difficultyText.setPosition(x, y);
}