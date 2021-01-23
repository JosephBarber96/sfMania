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
#include "DifficultySelectPannel.h"

SongSelectScene::SongSelectScene()
{
	m_backgroundSprite = new sf::Sprite();
	m_bannerTexture = new sf::Texture();
	m_bannerSprite = new sf::Sprite();
	m_songInfoBox = new SongInfoBox();
	m_songPanels = std::vector<SongPanel*>();
	m_difficultyPanels = std::vector<BeatmapDifficultyPanel*>();
	m_diffSelectMenu = new DifficultySelectMenu();
}


SongSelectScene::~SongSelectScene()
{
	delete m_songInfoBox;
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
	delete m_diffSelectMenu;
}




//--------------------------
// Scene
//--------------------------

void SongSelectScene::InitScene()
{
	int outline_thickness = 3;
	int gap = 5;

	// Self vars
	m_currentSongIndex = 0;
	SetMode(eSelectMode::songSelectMode);

	// Setup banner
	int b_x = (Settings::WindowX() * 0.5f) - (BANNER_WIDTH * 0.5f);
	int b_y = gap;
	m_bannerSprite->setPosition(b_x, b_y);

	// Setup Song Info Box
	m_songInfoBox->SetFillColour(Utility::UnhighlightedColour());
	m_songInfoBox->SetOutlineColour(sf::Color::Black);
	m_songInfoBox->SetOutlineThickness(outline_thickness);
	int sb_width = Settings::WindowX() * 0.8f;
	m_songInfoBox->SetSize(sb_width, SONG_INFOBOX_HEIGHT);
	int sb_x = (Settings::WindowX() * 0.5f) - (sb_width * 0.5f);
	int sb_y = b_y + gap + (int)(Settings::WindowY() / 5);
	m_songInfoBox->SetPosition(sb_x, sb_y);

	// Setup song boxes
	for (int i = 0; i < Song::SongCount(); i++)
	{
		SongPanel* panel = new SongPanel();

		int size = SongPanel::SIZE;
		int padding = PANEL_PADDING;

		int y = Settings::WindowY() - (size)-padding * 2;
		int x = (Settings::WindowX() * 0.5f) - (size * 0.5f) + (i * size) + (padding * (i + 1));

		panel->SetOutlineThickness(outline_thickness);
		panel->SetSize(size, size);
		panel->SetPosition(x, y);

		panel->SetInformation(Song::GetSong(i));

		if (i == m_currentSongIndex)
			panel->Highlight();
		else
			panel->UnHighlight();

		m_songPanels.push_back(panel);
	}

	// Load song info
	LoadInfoForSong();
}

void SongSelectScene::UnloadScene()
{
	AudioManager::StopMusic();
}

void SongSelectScene::UpdateScene()
{
	// Input

	if (m_mode == eSelectMode::songSelectMode)
	{
		// Go back
		if (Input::Esc.m_keyPressed)
		{
			GameManager::ChangeScene(eScenes::mainMenu);
		}

		// Scroll
		if (!m_scrolling)
		{
			// Advance song
			if (Input::Left.m_keyPressed && m_currentSongIndex > 0)
			{
				Scroll(eScrollDir::back);
			}

			// Go back song
			if (Input::Right.m_keyPressed && m_currentSongIndex < Song::SongCount() - 1)
			{
				Scroll(eScrollDir::forward);
			}

			// Select
			if (Input::Enter.m_keyPressed)
			{
				SetMode(eSelectMode::difficultySelectMode);
			}

			// Play audio after idle
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

		// Scrolling
		if (m_scrolling)
		{
			m_elapsedScrollTime += GameManager::DeltaTime();
			if (m_elapsedScrollTime >= SCROLL_TIME)
			{
				OnScrollFinished();
			}
		}
	}
	else if (m_mode == eSelectMode::difficultySelectMode)
	{
		// Go back
		if (Input::Esc.m_keyPressed)
		{
			SetMode(eSelectMode::songSelectMode);
		}

		// Select
		if (Input::Enter.m_keyPressed)
		{
			GameManager::SetChosenSong(m_currentSong, m_diffSelectMenu->ChosenDifficulty());
			GameManager::ChangeScene(eScenes::playingSong);
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

	if (m_mode == eSelectMode::difficultySelectMode)
	{
		m_diffSelectMenu->RenderSelf(window);
	}
}




//--------------------------
// SongSelectScene
//--------------------------

void SongSelectScene::Scroll(eScrollDir dir)
{
	m_scrolling = true;
	m_elapsedScrollTime = 0.0f;
	m_targetScrollIndex = (dir == eScrollDir::back) ? m_currentSongIndex - 1 : m_currentSongIndex + 1;

	m_songPanels[m_currentSongIndex]->UnHighlight();
	m_songPanels[m_targetScrollIndex]->Highlight();

	for (int i = 0; i < m_songPanels.size(); i++)
	{
		float yMove = 0; // PANEL_HEIGHT + PANEL_PADDING;
		float xMove = SongPanel::SIZE + PANEL_PADDING;

		if (dir == eScrollDir::forward)
		{
			yMove = 0 - yMove;
			xMove = 0 - xMove;
		}

		m_songPanels[i]->Scroll(SCROLL_TIME, xMove, yMove);
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
	m_backgroundSprite->setTexture(*AssetManager::GetTexture(eTexture::song_select_background));
	m_backgroundSprite->setColor(sf::Color(255, 255, 255, 128));
	auto backgroundScale = Utility::GetScaleForTargetSize(AssetManager::GetTexture(eTexture::song_select_background), Settings::WindowX(), Settings::WindowY());
	m_backgroundSprite->setScale(backgroundScale);

	// Banner
	m_bannerTexture->loadFromFile(m_currentSong->m_bannerFile);
	m_bannerSprite->setTexture(*m_bannerTexture);
	auto bannerScale = Utility::GetScaleForTargetSize(m_bannerTexture, BANNER_WIDTH, (int)(Settings::WindowY() / 5));
	m_bannerSprite->setScale(bannerScale);

	// Song info box
	m_songInfoBox->UpdateInformation(m_currentSong);

	// Delete current difficulty panels and create new ones
	for (int i = m_difficultyPanels.size() - 1; i > 0; i--)
	{
		delete m_difficultyPanels[i];
	}
	m_difficultyPanels.clear();

	int numOfDifficulties = m_currentSong->StepmapCount();
	for (int i = 0; i < numOfDifficulties; i++)
	{
		BeatmapDifficultyPanel* panel = new BeatmapDifficultyPanel(); 
		m_difficultyPanels.push_back(panel);

		int size = 75;
		int padding = 10;
		panel->SetSize(size, size);
		panel->SetOutlineColour(sf::Color::Black);
		panel->SetOutlineThickness(3);
		panel->SetDifficultyColour(Utility::GetDifficultyColour(i));
		panel->Highlight();
		panel->SetInfo(m_currentSong->GetStepmap(i));

		int diffXPos, diffYPos;
		// Even
		if (numOfDifficulties % 2 == 0)
		{
			int half = (numOfDifficulties / 2);
			int index = i + 1;
			int difference = abs(half - index);	
			int x;
			if (index <= half)
			{
				difference += 1;

				x = (Settings::WindowX() * 0.5f);
				x += size * -difference;
				x += padding * -difference;
				
			}
			else
			{
				x = (Settings::WindowX() * 0.5f);		
				x += size * (difference - 1);
				x += padding * (difference - 1);
			}

			diffXPos = x;

		}
		// Odd
		else
		{
			int middle = int(numOfDifficulties / 2) + 1;
			int indexDifference = (i + 1) - middle;
			int x = (Settings::WindowX() * 0.5f) - (size / 2);
			x -= size * indexDifference;
			x -= padding * (indexDifference + 1);

			diffXPos = x;
		}

		diffYPos = m_songInfoBox->GetY() + m_songInfoBox->GetHeight() + 10;

		panel->SetPosition(diffXPos, diffYPos);
	}
	m_difficultyPanels[0]->Highlight();
	m_currentDifficultyIndex = 0;
}

void SongSelectScene::SetMode(eSelectMode mode)
{
	// Exit 
	switch (m_mode)
	{
	case eSelectMode::songSelectMode:
		break;
	case eSelectMode::difficultySelectMode:
		m_diffSelectMenu->SetActive(false);
		break;
	}

	// Enter
	m_mode = mode;
	switch (m_mode)
	{
	case eSelectMode::songSelectMode:
		m_diffSelectMenu->SetActive(false);
		break;
	case eSelectMode::difficultySelectMode:
		m_diffSelectMenu->SetActive(true);
		m_diffSelectMenu->Init(Song::GetSong(m_currentSongIndex));
		break;
	}
}