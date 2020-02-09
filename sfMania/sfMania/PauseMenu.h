#pragma once

#include "MediaBox.h"

enum ePauseOptions : int
{
	Resume		=	0,
	Restart		=	1,
	Quit		=	2
};

class PauseMenu : public MediaBox
{
public:
	PauseMenu();
	~PauseMenu();

	void Init();
	void Update();
	void RenderSelf(sf::RenderWindow* window);

	// MediaBox
	void OnMediaBoxSetPosition();
	ePauseOptions CurrentOption() const { return (ePauseOptions)m_currentOption; }

private:

	void OnOptionChanged();

	int m_currentOption;

	sf::Text m_resumeText;
	sf::Text m_restartText;
	sf::Text m_quitText;

	sf::RectangleShape m_highlight;
};

