#include "DifficultySelectPannel.h"
#include "Song.h"
#include "Settings.h"
#include "Utility.h"
#include "StepMap.h"
#include "Input.h"
#include "DifficultyPanel.h"


DifficultySelectMenu::DifficultySelectMenu()
{
	m_difficulties = std::vector<DifficultyPanel*>();
}


DifficultySelectMenu::~DifficultySelectMenu()
{
	for (int i = 0; i < m_difficulties.size(); i++)
	{
		delete m_difficulties[i];
	}
}

void DifficultySelectMenu::Update()
{
	if (Input::Up.m_keyPressed && m_currentDifficulty > 0)
	{
		m_currentDifficulty--;
		UpdateHighlights();
	}
	else if (Input::Down.m_keyPressed && m_currentDifficulty < m_difficulties.size() - 1)
	{
		m_currentDifficulty++;
		UpdateHighlights();
	}
}

void DifficultySelectMenu::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(m_menuBox);

	for (int i = 0; i < m_difficulties.size(); i++)
	{
		m_difficulties[i]->RenderSelf(window);
	}
}

void DifficultySelectMenu::OnMediaBoxSetPosition()
{

}

void DifficultySelectMenu::Init(Song* song)
{
	m_song = song;

	// Background dim
	MediaBox::SetSize(Settings::WindowX(), Settings::WindowY());
	MediaBox::SetFillColour(Utility::UnhighlightedColour());

	// Menu
	m_menuBox = sf::RectangleShape();
	sf::Vector2f size = sf::Vector2f(350, 500);
	m_menuBox.setSize(size);
	int menuX = (Settings::WindowX() * 0.5f) - (size.x * 0.5f);
	int menuY = (Settings::WindowY() * 0.5f) - (size.y * 0.5f);
	m_menuBox.setPosition(menuX, menuY);
	m_menuBox.setOutlineColor(sf::Color::Black);
	m_menuBox.setOutlineThickness(3);
	m_menuBox.setFillColor(sf::Color::Black);

	// Difficulties
	for (int i = 0; i < m_difficulties.size(); i++)
	{
		delete m_difficulties[i];
	}
	m_difficulties.clear();
	int padding = 10;
	int numDifficulties = song->StepmapCount();
	sf::Vector2f difSize = sf::Vector2f(size.x * 0.8f, size.y * 0.2f);
	for (int i = 0; i < numDifficulties; i++)
	{
		StepMap* stepmap = song->GetStepmap(i);
		DifficultyPanel* panel = new DifficultyPanel();
		m_difficulties.push_back(panel);

		// Size
		panel->SetSize(difSize.x, difSize.y);

		// Pos
		int difX = (Settings::WindowX() * 0.5f) - (difSize.x * 0.5f);
		int difY = menuY + (padding * i) + (difSize.y * i);
		panel->SetPosition(difX, difY);

		// Colour
		sf::Color col = Utility::GetDifficultyColour(i);
		panel->SetOutlineColour(col);
		col.a = 128;
		panel->SetFillColour(col);
		panel->SetOutlineThickness(3);

		// Text
		panel->SetDifficultyText(std::to_string(stepmap->m_numericalDifficulty) + ": " + stepmap->m_difficultyName);		
	}
	UpdateHighlights();
}

void DifficultySelectMenu::UpdateHighlights()
{
	for (int i = 0; i < m_difficulties.size(); i++)
	{
		m_difficulties[i]->SetChosen(false);
	}
	m_difficulties[m_currentDifficulty]->SetChosen(true);
}