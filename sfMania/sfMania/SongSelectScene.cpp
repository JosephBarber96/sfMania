#include <iostream>
#include <algorithm>

#include "Utility.h"

#include "SongSelectScene.h"
#include "GameplayScene.h"

#include "Input.h"
#include "GameManager.h"
#include "SongInfoBox.h"
#include "SongPanel.h"
#include "Song.h"
#include "Settings.h"
#include "BeatmapDifficultyPanel.h"
#include "AudioManager.h"
#include "AssetManager.h"

SongSelectScene::SongSelectScene()
{
}


SongSelectScene::~SongSelectScene()
{
}




//--------------------------
// Scene
//--------------------------

void SongSelectScene::InitScene()
{
	int outline_thickness = 3;

	// Self vars
	m_currentSongIndex = 0;

	// Setup background
	m_backgroundTexture = new sf::Texture();
	m_backgroundSprite = new sf::Sprite();

	// Setup banner
	m_bannerTexture = new sf::Texture();
	m_bannerSprite = new sf::Sprite();

	// Setup Song Info Box
	m_songInfoBox = new SongInfoBox();
	m_songInfoBox->SetFillColour(Utility::UnhighlightedColour());
	m_songInfoBox->SetOutlineColour(sf::Color::Black);
	m_songInfoBox->SetOutlineThickness(outline_thickness);
	m_songInfoBox->SetPosition(5, 205);
	m_songInfoBox->SetSize(BANNER_WIDTH, SONG_INFOBOX_HEIGHT);

	// Setup song panels
	m_songPanels = std::vector<SongPanel*>();
	for (int i = 0; i < Song::SongCount(); i++)
	{
		SongPanel* panel = new SongPanel();

		int wid, hei, x, y, padding;
		hei = PANEL_HEIGHT;
		padding = PANEL_PADDING;
		wid = Settings::WindowX() * 0.45f;
		x = Settings::WindowX() * 0.55f;
		y = Settings::WindowY() * 0.5f + (i * hei) + padding * (i + 1);

		panel->SetOutlineColour(sf::Color::Black);
		panel->SetOutlineThickness(outline_thickness);
		panel->SetSize(wid, hei);
		panel->SetPosition(x, y);

		panel->SetInformation(Song::GetSong(i));

		if (i == m_currentSongIndex)
			panel->Highlight();
		else
			panel->UnHighlight();

		m_songPanels.push_back(panel);
	}

	// Setup beatmap panels
	m_difficultyPanels = std::vector<BeatmapDifficultyPanel*>();

	// Load song info
	LoadInfoForSong();
}

void SongSelectScene::UnloadScene()
{
	delete m_songInfoBox;
	delete m_backgroundTexture;
	delete m_backgroundSprite;
	delete m_bannerTexture;
	delete m_bannerSprite;
	for (int i = 0; i < m_songPanels.size(); i++)
	{
		delete m_songPanels[i];
	}
	for (int i = 0; i < m_difficultyPanels.size(); i++)
	{
		delete m_difficultyPanels[i];
	}

	AudioManager::StopMusic();
}

void SongSelectScene::UpdateScene()
{
	// Input

	// Go back
	if (Input::Esc.m_keyPressed)
	{
		GameManager::ChangeScene(eScenes::mainMenu);
	}

	// Scroll
	if (!m_scrolling)
	{
		// Advance song
		if (Input::Right.m_keyPressed && m_currentSongIndex < Song::SongCount() - 1)
		{
			Scroll(eScrollDir::up);
		}

		// Go back song
		if (Input::Left.m_keyPressed && m_currentSongIndex > 0)
		{
			Scroll(eScrollDir::down);
		}
	}

	//Difficulty select
	if (Input::Up.m_keyPressed)
	{
		if (m_currentDifficultyIndex > 0)
		{
			m_difficultyPanels[m_currentDifficultyIndex]->UnHighlight();
			m_currentDifficultyIndex--;
			m_difficultyPanels[m_currentDifficultyIndex]->Highlight();
		}
	}
	else if (Input::Down.m_keyPressed)
	{
		if (m_currentDifficultyIndex < m_difficultyPanels.size() - 1)
		{
			m_difficultyPanels[m_currentDifficultyIndex]->UnHighlight();
			m_currentDifficultyIndex++;
			m_difficultyPanels[m_currentDifficultyIndex]->Highlight();
		}
	}

	// Select
	if (Input::Enter.m_keyPressed)
	{
		GameManager::SetChosenSong(m_currentSong, m_currentDifficultyIndex);
		GameManager::ChangeScene(eScenes::playingSong);
	}


	// Scrolling
	if (m_scrolling)
	{
		m_elapsedScrollTime += GameManager::DeltaTime();
		if (m_elapsedScrollTime >= SCROLL_TIME)
		{
			OnScrollFinished();
		}
	}

	if (!m_scrolling && !m_songPlaying)
	{
		m_timeOnSelectedSong += GameManager::DeltaTime();
		if (m_timeOnSelectedSong >= TIME_BEFORE_SONG)
		{
			m_songPlaying = true;
			AudioManager::PlayMusic(m_currentSong);
		}
	}
}

void SongSelectScene::UpdateSceneTransition(float normalized)
{

}

void SongSelectScene::RenderScene(sf::RenderWindow* window)
{
	window->draw(*m_backgroundSprite);
	window->draw(*m_bannerSprite);

	m_songInfoBox->RenderSelf(window);

	for (int i = 0; i < m_songPanels.size(); i++)
	{
		m_songPanels[i]->RenderSelf(window);
	}

	for (int i = 0; i < m_difficultyPanels.size(); i++)
	{
		m_difficultyPanels[i]->RenderSelf(window);
	}
}




//--------------------------
// SongSelectScene
//--------------------------

void SongSelectScene::Scroll(eScrollDir dir)
{
	m_scrolling = true;
	m_elapsedScrollTime = 0.0f;
	m_targetScrollIndex = (dir == eScrollDir::up) ? m_currentSongIndex + 1 : m_currentSongIndex - 1;

	m_songPanels[m_currentSongIndex]->UnHighlight();
	m_songPanels[m_targetScrollIndex]->Highlight();

	for (int i = 0; i < m_songPanels.size(); i++)
	{
		m_songPanels[i]->Scroll(dir, SCROLL_TIME, PANEL_HEIGHT + PANEL_PADDING);
	}

	AudioManager::PlaySound(eSound::scroll);
}

void SongSelectScene::OnScrollFinished()
{
	m_scrolling = false;
	m_currentSongIndex = m_targetScrollIndex;
	LoadInfoForSong();
	AudioManager::StopMusic();
	m_timeOnSelectedSong = 0;
	m_songPlaying = false;
}

void SongSelectScene::LoadInfoForSong()
{
	m_currentSong = Song::GetSong(m_currentSongIndex);

	float width, height;
	float widthScale, heightScale;
	float targetWidth, targetHeight;

	// Background
	m_backgroundTexture->loadFromFile(m_currentSong->m_backgroundFile);
	m_backgroundSprite->setTexture(*m_backgroundTexture);
	m_backgroundSprite->setColor(sf::Color(255, 255, 255, 100));
	auto backgroundScale = Utility::GetScaleForTargetSize(m_backgroundTexture, Settings::WindowX(), Settings::WindowY());
	m_backgroundSprite->setScale(backgroundScale);

	// Banner
	m_bannerTexture->loadFromFile(m_currentSong->m_bannerFile);
	m_bannerSprite->setTexture(*m_bannerTexture);
	auto bannerScale = Utility::GetScaleForTargetSize(m_bannerTexture, BANNER_WIDTH, (int)(Settings::WindowY() / 5));
	m_bannerSprite->setScale(bannerScale);
	m_bannerSprite->setPosition(5, 5);

	// Song info box
	int sb_x, sb_y;
	sb_x = m_bannerSprite->getPosition().x;
	sb_y = m_bannerSprite->getPosition().y + m_bannerSprite->getGlobalBounds().height + 5;
	m_songInfoBox->SetPosition(sb_x, sb_y);
	m_songInfoBox->UpdateInformation(m_currentSong);

	// Delete current difficulty panels and create new ones
	for (int i = m_difficultyPanels.size() - 1; i > 0; i--)
	{
		delete m_difficultyPanels[i];
	}
	m_difficultyPanels.clear();

	int padding = 10;
	int diffPanelHeight = 45;
	int panel_y = sb_y + SONG_INFOBOX_HEIGHT + padding;
	for (int i = 0; i < m_currentSong->StepmapCount(); i++)
	{
		BeatmapDifficultyPanel* panel = new BeatmapDifficultyPanel();
		panel->SetSize(BANNER_WIDTH, diffPanelHeight);
		panel->SetPosition(sb_x, panel_y + padding * i + diffPanelHeight * i);
		panel->SetOutlineColour(sf::Color::Black);
		panel->SetOutlineThickness(3);
		panel->SetFillColour(Utility::UnhighlightedColour());
		panel->SetInfo(m_currentSong->GetStepmap(i));
		m_difficultyPanels.push_back(panel);
	}
	m_difficultyPanels[0]->Highlight();
	m_currentDifficultyIndex = 0;
}