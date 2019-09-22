#include "ResultsScene.h"

#include "GameManager.h"
#include "GameplayScene.h"
#include "Settings.h"
#include "Song.h"
#include "Score.h"
#include "Input.h"
#include "Utility.h"
#include "ResourceManager.h"

Score* ResultsScene::score;


ResultsScene::ResultsScene()
{
}


ResultsScene::~ResultsScene()
{
}




//--------------------------
// Scene
//--------------------------

void ResultsScene::InitScene()
{
	// Get song
	Song* currentSong = GameManager::GetCurrentSong();
	
	// Score
	m_scoreText = sf::Text();
	sf::Color fillColor;
	std::string scoreString;
	switch (score->CalculateScore())
	{
	case eGrade::AAA:
		scoreString = ("AAA");
		fillColor = (sf::Color::Yellow);
		break;

	case eGrade::AA:
		scoreString = ("AA");
		fillColor = (sf::Color::Yellow);
		break;

	case eGrade::A:
		scoreString = ("A");
		fillColor = (sf::Color::Green);
		break;

	case eGrade::B:
		scoreString = ("B");
		fillColor = (sf::Color::Blue);
		break;

	case eGrade::C:
		scoreString = ("C");
		fillColor = (sf::Color::Magenta);
		break;

	case eGrade::D:
		scoreString = ("D");
		fillColor = (sf::Color(255, 128, 0));
		break;

	case eGrade::E:
		scoreString = ("E");
		fillColor = (sf::Color::Red);
		break;
	}
	Utility::SetupText(&m_scoreText, ResourceManager::GetFont(eFont::big), scoreString, 300, fillColor, sf::Color::Black, 3, 50, -25);


	float startTextY = 0.55f;
	float incrementY = 0.1f;
	int i = 0;
	int titleX = Settings::WindowX() * 0.45f;
	int actualX = Settings::WindowX() * 0.55f;
	int fontSize = 32;

	// MaxCombo
	m_maxComboTitle = sf::Text();
	Utility::SetupText(&m_maxComboTitle, ResourceManager::GetFont(eFont::big), "Max combo:", fontSize, sf::Color::Cyan, sf::Color::Black, 2, 0, Settings::WindowY() * (startTextY + (i * incrementY)));
	m_maxComboTitle.setPosition(titleX - m_maxComboTitle.getGlobalBounds().width, m_maxComboTitle.getPosition().y);
	m_maxCombo = sf::Text();
	Utility::SetupText(&m_maxCombo, ResourceManager::GetFont(eFont::big), std::to_string(score->GetMaxCombo()), fontSize, sf::Color::White, sf::Color::Black, 2, actualX, Settings::WindowY() * (startTextY + (i++ * incrementY)));

	// Perfect notes
	m_perfectNotesTitle = sf::Text();
	Utility::SetupText(&m_perfectNotesTitle, ResourceManager::GetFont(eFont::big), "Perfect:", fontSize, sf::Color::Yellow, sf::Color::Black, 2, 0, Settings::WindowY() * (startTextY + (i * incrementY)));
	m_perfectNotesTitle.setPosition(titleX - m_perfectNotesTitle.getGlobalBounds().width, m_perfectNotesTitle.getPosition().y);
	m_perfectNotes = sf::Text();
	Utility::SetupText(&m_perfectNotes, ResourceManager::GetFont(eFont::big), std::to_string(score->GetPerfectNotes()), fontSize, sf::Color::White, sf::Color::Black, 2, actualX, Settings::WindowY() * (startTextY + (i++ * incrementY)));
	
	// Great notes
	m_greatNotesTitle = sf::Text();
	Utility::SetupText(&m_greatNotesTitle, ResourceManager::GetFont(eFont::big), "Great:", fontSize, sf::Color::Green, sf::Color::Black, 2, 0, Settings::WindowY() * (startTextY + (i * incrementY)));
	m_greatNotesTitle.setPosition(titleX - m_greatNotesTitle.getGlobalBounds().width, m_greatNotesTitle.getPosition().y);
	m_greatNotes = sf::Text();
	Utility::SetupText(&m_greatNotes, ResourceManager::GetFont(eFont::big), std::to_string(score->GetGreatNotes()), fontSize, sf::Color::White, sf::Color::Black, 2, actualX, Settings::WindowY() * (startTextY + (i++ * incrementY)));

	// Misses
	m_missNotesTitle = sf::Text();
	Utility::SetupText(&m_missNotesTitle, ResourceManager::GetFont(eFont::big), "Misses:", fontSize, sf::Color::Red, sf::Color::Black, 2, 0, Settings::WindowY() * (startTextY + (i * incrementY)));
	m_missNotesTitle.setPosition(titleX - m_missNotesTitle.getGlobalBounds().width, m_missNotesTitle.getPosition().y);
	m_missNotes = sf::Text();
	Utility::SetupText(&m_missNotes, ResourceManager::GetFont(eFont::big), std::to_string(score->GetMissNotes()), fontSize, sf::Color::White, sf::Color::Black, 2, actualX, Settings::WindowY() * (startTextY + (i++ * incrementY)));

	// Box
	m_scoreBox = sf::RectangleShape();
	m_scoreBox.setFillColor(sf::Color(255, 255, 255, 50));
	m_scoreBox.setSize(sf::Vector2f(Settings::WindowX() * 0.95f, Settings::WindowY() * 0.45f));
	m_scoreBox.setPosition(Settings::WindowX() * 0.025f, Settings::WindowY() * 0.5f);
	m_scoreBox.setOutlineColor(sf::Color::Black);
	m_scoreBox.setOutlineThickness(2);

	// BG
	m_backgroundTexture = new sf::Texture();
	m_backgroundTexture->loadFromFile(currentSong->m_backgroundFile);
	m_backgroundTexture->setSmooth(true);
	m_backgroundSprite = new sf::Sprite();
	m_backgroundSprite->setTexture(*m_backgroundTexture);
	m_backgroundSprite->setColor(sf::Color(255, 255, 255, 50));
	m_backgroundSprite->setScale(Utility::GetScaleForTargetSize(m_backgroundTexture, Settings::WindowX(), Settings::WindowY()));
}

void ResultsScene::UnloadScene()
{
	delete m_backgroundTexture;
	delete m_backgroundSprite;
	delete ResultsScene::score;
}

void ResultsScene::UpdateScene()
{
	if (Input::Esc.m_keyPressed)
	{
		GameManager::ChangeScene(eScenes::songSelect);
	}
}

void ResultsScene::RenderScene(sf::RenderWindow * window)
{
	window->draw(*m_backgroundSprite);

	window->draw(m_scoreText);

	window->draw(m_scoreBox);

	window->draw(m_maxComboTitle);
	window->draw(m_maxCombo);

	window->draw(m_perfectNotesTitle);
	window->draw(m_perfectNotes);

	window->draw(m_greatNotesTitle);
	window->draw(m_greatNotes);

	window->draw(m_missNotesTitle);
	window->draw(m_missNotes);
}




//--------------------------
// ResultsScene
//--------------------------

// Public

void ResultsScene::LoadSceneResults(Score* score)
{
	ResultsScene::score = score;
	GameManager::ChangeScene(eScenes::resultsScreen);
}