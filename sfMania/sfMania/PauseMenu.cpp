#include "PauseMenu.h"

#include "Settings.h"
#include "Utility.h"
#include "ResourceManager.h"
#include "Input.h"

PauseMenu::PauseMenu()
{
	m_currentOption = ePauseOptions::Resume;
}

PauseMenu::~PauseMenu() { }

void PauseMenu::Init()
{
	SetSize(Settings::WindowX(), Settings::WindowY());
	SetPosition(0, 0);
	SetFillColour(Utility::UnhighlightedColour());

	int fontSize = 30;
	Utility::SetupText(&m_resumeText, eFont::big, "Resume", fontSize, sf::Color::White, sf::Color::Black, 2);
	Utility::SetupText(&m_restartText, eFont::big, "Restart", fontSize, sf::Color::White, sf::Color::Black, 2);
	Utility::SetupText(&m_quitText, eFont::big, "Quit", fontSize, sf::Color::White, sf::Color::Black, 2);

	m_resumeText.setPosition(Utility::GetScreenPosForTextForNormalizedScreenCoordinate(&m_resumeText, 0.5f, 0.4f));
	m_restartText.setPosition(Utility::GetScreenPosForTextForNormalizedScreenCoordinate(&m_restartText, 0.5f, 0.5f));
	m_quitText.setPosition(Utility::GetScreenPosForTextForNormalizedScreenCoordinate(&m_quitText, 0.5f, 0.6f));

	m_highlight = sf::RectangleShape();
	m_highlight.setFillColor(Utility::HighlightedColour());
	m_highlight.setSize(sf::Vector2f(Settings::WindowX() * 0.35f, Settings::WindowY() * 0.075f));
	OnOptionChanged();
}

void PauseMenu::Update()
{
	if (Input::Down.m_keyPressed)
	{
		if (m_currentOption != ePauseOptions::Quit)
		{
			m_currentOption++;
			OnOptionChanged();
		}
	}
	else if (Input::Up.m_keyPressed)
	{
		if (m_currentOption != ePauseOptions::Resume)
		{
			m_currentOption--;
			OnOptionChanged();
		}
	}
}

void PauseMenu::RenderSelf(sf::RenderWindow* window)
{
	MediaBox::RenderSelf(window);

	window->draw(m_highlight);

	window->draw(m_resumeText);
	window->draw(m_restartText);
	window->draw(m_quitText);
}

void PauseMenu::OnMediaBoxSetPosition()
{

}

// Private

void PauseMenu::OnOptionChanged()
{
	sf::Vector2f pos = m_highlight.getPosition();
	float width = m_highlight.getGlobalBounds().width;
	float height = m_highlight.getGlobalBounds().height;
	float windowHeight = Settings::WindowY();

	switch (m_currentOption)
	{
	case ePauseOptions::Resume:
		pos.y = windowHeight * 0.4f;
		break;

	case ePauseOptions::Restart:
		pos.y = windowHeight * 0.5f;
		break;

	case ePauseOptions::Quit:
		pos.y = windowHeight * 0.6f;
		break;
	}

	pos.x = (Settings::WindowX() * 0.5f) - width * 0.5f;
	pos.y -= height * 0.375f;

	m_highlight.setPosition(pos);
}