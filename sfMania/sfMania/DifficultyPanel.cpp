#include "DifficultyPanel.h"
#include "Utility.h"
#include "AssetManager.h"

DifficultyPanel::DifficultyPanel()
{
	m_difficultyText = sf::Text();
	Utility::SetupText(&m_difficultyText, AssetManager::GetFont(eFont::small), "Easy", 25, sf::Color::White, sf::Color::Black, 2);
}


DifficultyPanel::~DifficultyPanel()
{

}

// GameObject
void DifficultyPanel::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);
	window->draw(m_difficultyText);
}

// MediaBox
void DifficultyPanel::OnMediaBoxSetPosition()
{
	PositionText();
}

// DifficultyPanel
void DifficultyPanel::SetChosen(bool chosen)
{
	sf::Color fillCol = m_box.getFillColor();
	if (chosen)
	{
		fillCol.a = 255;
	}
	else
	{
		fillCol.a = 128;
	}
	MediaBox::SetFillColour(fillCol);
}

void DifficultyPanel::SetDifficultyText(std::string str)
{
	m_difficultyText.setString(str);
	PositionText();
}

void DifficultyPanel::PositionText()
{
	int x = Utility::GetXForText(&m_difficultyText, 0.5f);
	int text_height = m_difficultyText.getGlobalBounds().height;
	int y = m_y + (m_box.getGlobalBounds().height * 0.5f) - (text_height * 0.5f);

	m_difficultyText.setPosition(x, y);
}