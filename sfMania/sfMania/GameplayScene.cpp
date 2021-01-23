#include <iostream>
#include <string>

#include "GameplayScene.h"

#include "GameManager.h"
#include "AssetManager.h"
#include "Utility.h"
#include "Settings.h"
#include "ResultsScene.h"
#include "Input.h"
#include "Maths.h"

#include "PauseMenu.h"

#include "Song.h"
#include "StepMap.h"
#include "Measure.h"
#include "Line.h"
#include "BPM.h"
#include "Score.h"
#include "HealthBar.h"
#include "ProgressBar.h"

#include "Receptor.h"
#include "Note.h"
#include "LongNote.h"
#include "SceneChangeArgs.h"

// INFORMATION
/*
1 Measure = 4 beats.

Therefore, a 4 measure is 4 beats:
0000 <- 1
0000 <- 2
0000 <- 3
0000 <- 4

An 8 measure is still 4 beats long
0000 <- 1
0000
0000 <- 2
0000
0000 <- 3
0000
0000 <- 4
0000

These are the beats that BPM changes occur on, so this is
how we calculate what beat we're currently on.
*/


GameplayScene::GameplayScene()
{
	m_notes = std::vector<Note*>();
	m_longNotes = std::vector<LongNote*>();
	m_activeLongNotes = std::array<LongNote*, 4>();
	m_receptors = std::array<Receptor*, 4>();
}


GameplayScene::~GameplayScene()
{
	// Delete heap-allocated notes
	int sz = m_notes.size();
	for (int i = sz - 1; i > 0; i--)
	{
		delete m_notes[i];
		m_notes.erase(m_notes.begin() + i);
	}
	// Delete heap-allocated longNotes
	sz = m_longNotes.size();
	for (int i = sz - 1; i > 0; i--)
	{
		delete m_longNotes[i];
		m_longNotes.erase(m_longNotes.begin() + i);
	}

	// Delete heap objects
	for (int i = 0; i < 4; i++)
		delete m_receptors[i];
	delete m_songMusic;
	delete m_backgroundTexture;
	delete m_backgroundSprite;
	delete m_playBox;
	delete m_comboText;
	delete m_hitText;
	delete m_pauseMenu;
	delete m_healthBar;
	delete m_progressBar;
	delete m_score;
}




//--------------------------
// Scene
//--------------------------

void GameplayScene::InitScene(SceneChangeArgs* args)
{
	// Args 
	SongSceneChangeArgs* newArgs = static_cast<SongSceneChangeArgs*>(args);

	// Init vars
	m_score = new Score();
	m_paused = false;

	// Get song + stepmap
	m_songIndex = newArgs->chosenSongIndex;
	m_diffIndex = newArgs->chosenDifficultyIndex;
	m_currentSong = Song::GetSong(newArgs->chosenSongIndex);
	m_stepMap = m_currentSong->GetStepmap(newArgs->chosenDifficultyIndex);

	for (int i = 0; i < 4; i++)
	{
		m_receptors[i] = new Receptor();
		m_receptors[i]->InitSelf(i, GetXForColumn(i));
	}

	// Init music
	m_songMusic = new sf::Music();

	// Init notes pool
	for (int i = 0; i < 50; i++)
	{
		m_notes.push_back(new Note());
		m_longNotes.push_back(new LongNote());		
	}
	m_currentNoteIndex = 0;
	m_currentLongnoteIndex = 0;
	Note::InitSceneRef(this);

	// Init UI
	{
		// BG
		m_backgroundTexture = new sf::Texture();
		m_backgroundTexture->loadFromFile(m_currentSong->m_backgroundFile);
		m_backgroundTexture->setSmooth(true);
		m_backgroundSprite = new sf::Sprite();
		m_backgroundSprite->setTexture(*m_backgroundTexture);
		m_backgroundSprite->setScale(Utility::GetScaleForTargetSize(m_backgroundTexture, Settings::WindowX(), Settings::WindowY()));
		m_backgroundSprite->setColor(sf::Color(255, 255, 255, 50));

		// Playbox
		m_playBox = new sf::RectangleShape();
		m_playBox->setSize(sf::Vector2f(Settings::WindowX() / 2, Settings::WindowY()));
		m_playBox->setFillColor(sf::Color());
		m_playBox->setOutlineColor(sf::Color(255, 255, 255, 50));
		m_playBox->setOutlineThickness(2);
		m_playBox->setPosition(Settings::WindowX() / 4, 0);

		// Combo text
		m_comboText = new sf::Text();
		m_comboText->setFont(*AssetManager::GetFont(eFont::bold));
		m_comboTextPos = sf::Vector2i(
			(Settings::WindowX() / 2) - (m_comboText->getGlobalBounds().width / 2),
			(Settings::WindowY() * 0.75f) - (Settings::WindowY() / 2) - (m_comboText->getGlobalBounds().height / 2)
		);
		m_comboText->setString("");
		m_comboText->setOrigin((m_comboText->getGlobalBounds().width / 2), (m_comboText->getGlobalBounds().height / 2));
		m_comboText->setFillColor(sf::Color(255, 165, 0, 255));
		m_comboText->setOutlineColor(sf::Color(0, 0, 0, 178));
		m_comboText->setOutlineThickness(3);
		m_comboText->setCharacterSize(70);
		

		// Hit text
		m_hitText = new sf::Text();
		m_hitText->setFont(*AssetManager::GetFont(eFont::bold));
		m_hitTextPos = sf::Vector2i(
			(Settings::WindowX() / 2) - (m_hitText->getGlobalBounds().width / 2),
			(Settings::WindowY() / 2) - (m_hitText->getGlobalBounds().height / 2)
		);
		m_hitText->setString("");
		m_hitText->setOrigin((m_hitText->getGlobalBounds().width / 2), (m_hitText->getGlobalBounds().height / 2));
		m_hitText->setOutlineColor(sf::Color(0, 0, 0, 178));
		m_hitText->setOutlineThickness(3);
		m_hitText->setCharacterSize(70);

		// Text related vars
		m_targetComboExpandTime = 0.15f;
	}

	// Health bar
	m_healthBar = new HealthBar(MAX_HEALTH);
	m_healthBar->SetupPosition();

	// Progress bar 
	m_progressBar = new ProgressBar();
	m_progressBar->SetupPosition();

	// Pause menu
	m_pauseMenu = new PauseMenu();
	m_pauseMenu->Init();
	m_pauseMenu->IsActive();

	// Vars
	m_songPlaying = false;

	Play();
}

void GameplayScene::UnloadScene()
{
	// Reset vars
	m_paused = false;
}

void GameplayScene::UpdateScene()
{
	if (!m_paused)
	{
		if (m_songPlaying)
		{
			UpdateReceptorInput();
			if (!m_endOfNotes)
				UpdateSongAndNotes();
			UpdateComboText();
		}


		// Progress bar
		float prog = m_songMusic->getPlayingOffset() / m_songMusic->getDuration();
		m_progressBar->SetProgressNormalised(prog);

		// Has the song ended?
		if (m_endOfNotes && m_songMusic->getStatus() == sf::SoundSource::Status::Stopped)
		{
			LeaveScene(eLeaveSongReason::songEnd);
			return;
		}

		// Has player died?
		if (m_healthBar->IsDead())
		{
			LeaveScene(eLeaveSongReason::dead);
			return;
		}
	}
	else
	{
		UpdatePaused();
	}


	// Quit
	if (Input::Esc.m_keyPressed)
	{
		TogglePause();
	}
}

void GameplayScene::UpdateSceneTransition(float normalized)
{

}

void GameplayScene::RenderScene(sf::RenderWindow * window)
{
	// Background
	window->draw(*m_backgroundSprite);
	// Playbox
	window->draw(*m_playBox);

	// Receptors
	for (int i = 0; i < 4; i++)
		m_receptors[i]->RenderSelf(window);

	// Notes
	for (int i = 0; i < m_notes.size(); i++)
	{
		m_notes[i]->RenderSelf(window);
	}

	// LongNotes
	for (int i = 0; i < m_longNotes.size(); i++)
	{
		m_longNotes[i]->RenderSelf(window);
	}

	// Healthbar
	m_healthBar->RenderSelf(window);

	// Progress bar
	m_progressBar->RenderSelf(window);

	// Combo text
	window->draw(*m_comboText);
	window->draw(*m_hitText);

	// Paused
	if (m_paused)
	{
		m_pauseMenu->RenderSelf(window);
	}
}




//--------------------------
// GameplayScene
//--------------------------

// Public

void GameplayScene::Play()
{
	// Load the song
	if (!m_songMusic->openFromFile(m_currentSong->m_musicFile))
	{
		std::cout << "ERROR: Cannot load song from file." << std::endl;
	}

	m_currentBeat = 0;
	m_curMeasureIndex = 0;
	m_curLineIndex = 0;
	m_currentBpm = 0;
	m_nextLineDelay = 0;

	// Init var info
	m_curBeatLine = 0;
	int numberOfLines = m_stepMap->GetMeasure(m_curMeasureIndex)->LineCount();
	m_linesPerBeat = numberOfLines / 4;

	// Calculate offset
	float songOffset = m_currentSong->m_offset;
	float noteOffset = Settings::FallTime();
	
	// Case 0: Positive or 0 offset
	if (songOffset >= 0)
	{
		std::cout << "Case 0:" << std::endl;
		float newOffset = songOffset + noteOffset;

		// Play notes straight away
		m_initialNoteDelay = 0;
		// Wait before song
		m_initialSongDelay = newOffset;
	}

	// Case 1: 
	// Negative offset BUT noteOffset > abs(songOffset)
	//
	// Therefore: noteOffset - abs(songOffet) > 0
	if (songOffset < 0 && noteOffset > fabsf(songOffset))
	{
		std::cout << "Case 1:" << std::endl;
		// Instead of having beat0 occur after the song
		// It now occurs before the song
		float newOffset = noteOffset - fabsf(songOffset);

		// Play notes straight away
		m_initialNoteDelay = 0;
		// Wait before song
		m_initialSongDelay = newOffset;
	}

	// Case 2:
	// Negative offset BUT noteOffset < abs(songOffset)
	//
	// Therefore: noteOffset - abs(songOffset) < 0
	else if (songOffset < 0 && noteOffset < fabsf(songOffset))
	{
		std::cout << "Case 2:" << std::endl;
		float newOffset = (fabsf(songOffset)) - noteOffset;

		// Play song straight away
		m_initialSongDelay = 0;
		// Wait before notes
		m_initialNoteDelay = newOffset;
	}

	// Init playing
	m_songPlaying = true;
	m_endOfNotes = false;
	m_songStarted = false;
}

void GameplayScene::AlertNoteMissed()
{
	NoteMissed();
}


// Private

void GameplayScene::UpdateReceptorInput()
{
	// Receptor pressed
	if (Input::W.m_keyPressed)
	{
		CheckForHit(0);
		m_receptors[0]->Pressed();
	}
	if (Input::E.m_keyPressed)
	{
		CheckForHit(1);
		m_receptors[1]->Pressed();
	}
	if (Input::I.m_keyPressed)
	{
		CheckForHit(2);
		m_receptors[2]->Pressed();
	}
	if (Input::O.m_keyPressed)
	{
		CheckForHit(3);
		m_receptors[3]->Pressed();
	}


	// Receptor released
	if (Input::W.m_keyReleased)
		m_receptors[0]->Released();
	if (Input::E.m_keyReleased)
		m_receptors[1]->Released();
	if (Input::I.m_keyReleased)
		m_receptors[2]->Released();
	if (Input::O.m_keyReleased)
		m_receptors[3]->Released();
}

void GameplayScene::UpdateSongAndNotes()
{
	float delta = GameManager::DeltaTime();

	// Play music
	if (!m_songStarted && m_initialSongDelay <= 0)
	{
		m_songStarted = true;
		m_songMusic->play();
	}

	// Can play notes
	if (m_initialNoteDelay <= 0)
	{
		if (!m_init)
		{
			m_curMeasureIndex = 0;
			m_curMeasure = m_stepMap->GetMeasure(m_curMeasureIndex);
			m_curLineIndex = 0;
			m_curBeatLine = 0;
			m_currentBpm = -1;

			m_nextLineDelay = 0;

			m_init = true;
		}

		if (m_init && m_curMeasure == nullptr)
			return;

		// Time has passed for the next notes to fall
		if (m_init && m_elapsedSongTime >= m_nextNoteDropTime)
		{
			// Drop notes for each column
			for (int i = 0; i < 4; i++)
			{
				switch (m_curMeasure->GetLine(m_curLineIndex)->GetChar(i))
				{
					// Regular arrow
				case '1':
					DropNote(i);
					break;

					// Long-note head
				case '2':
					// FGor now, treat rolls as long notes
				case '4':
					m_activeLongNotes[i] = DropLongNote(i);
					break;

					// Long-note tail
				case '3':
					m_activeLongNotes[i]->EndTail();
					break;
				}
			}

			// Go to next line
			m_curLineIndex++;			

			// Go to next measure?
			if (m_curLineIndex >= m_stepMap->GetMeasure(m_curMeasureIndex)->LineCount())
			{
				m_curLineIndex = 0;
				m_curMeasureIndex++;
				m_curMeasure = m_stepMap->GetMeasure(m_curMeasureIndex);
				if (m_curMeasure == nullptr)
				{
					m_endOfNotes = true;
					return;
				}

				// Every {linesPerBeat} number of lines, we count beatCount++;
				m_curBeatLine = 0;
				int numberOfLines = m_stepMap->GetMeasure(m_curMeasureIndex)->LineCount();
				m_linesPerBeat = numberOfLines / 4;
			}
			// Else, on line increment
			else
			{
				// Count lines per beat, move onto next beat if need to			
				if (m_curBeatLine == m_linesPerBeat - 1)
				{
					m_curBeatLine = 0;
					m_currentBeat++;
				}
				m_curBeatLine++;
			}

			// Calculate how long we have to wait { at the current BPM and Measure }
			// untill we process the next line
			{
				// BPM of current beat
				float bpm = m_currentSong->GetBpmForBeat(m_currentBeat)->Bpm();
				if (m_currentBpm != bpm)
				{
					m_currentBpm = bpm;
					std::cout << "Switching to " << m_currentBpm << " bpm!" << std::endl;
				}

				// Beats per sec
				float current_bps = m_currentBpm / 60;

				// how long between each hit
				float current_wait_time = 1.0f / current_bps;

				// hits per metrinome
				float hits_per_bpm = m_linesPerBeat;

				// wait pet beat
				float wait_time_per_beat = current_wait_time / hits_per_bpm;

				m_nextNoteDropTime = (m_nextNoteDropTime + wait_time_per_beat);
			}
		}

		m_elapsedSongTime += delta;
	}

	

	m_initialNoteDelay -= delta;
	m_initialSongDelay -= delta;
}

void GameplayScene::UpdateComboText()
{
	// Update text
	if (m_expandCombo)
	{
		// Time, normalized
		m_elapsedComboExpandTime += GameManager::DeltaTime();
		float normalized = m_elapsedComboExpandTime / m_targetComboExpandTime;
		normalized = Maths::Clamp(normalized, 0, 1);

		// Set text
		float scale = Maths::Unnormalize(normalized, 1.15f, 1.0f);
		m_comboText->setScale(scale, scale);
		m_hitText->setScale(scale, scale);

		// End catch
		if (normalized >= 1)
			m_expandCombo = false;
	}
}

void GameplayScene::DropNote(int column)
{
	m_notes[m_currentNoteIndex]->BeginDrop(GetXForColumn(column), column);
	m_currentNoteIndex++;

	if (m_currentNoteIndex >= m_notes.size() - 1)
		m_currentNoteIndex = 0;
}

LongNote* GameplayScene::DropLongNote(int column)
{
	LongNote* ret = m_longNotes[m_currentLongnoteIndex];
	ret->BeginDrop(GetXForColumn(column), column);
	m_currentLongnoteIndex++;

	if (m_currentLongnoteIndex >= m_longNotes.size() - 1)
		m_currentLongnoteIndex = 0;

	return ret;
}

void GameplayScene::CheckForHit(int column)
{
	float timing_window = 0.1f; // 1/10th of a second

	// Get the lowest-down note in the given column
	Note * furthestNote = nullptr;
	float furthestFallTime;
	for (Note * note : m_notes)
	{
		if (note->IsActive() && note->Column() == column)
		{
			if (furthestNote == nullptr)
			{
				furthestNote = note;
				furthestFallTime = note->ElapsedFallTime();
			}
			else if (note->ElapsedFallTime() > furthestFallTime)
			{
				furthestNote = note;
				furthestFallTime = note->ElapsedFallTime();
			}
		}
	}

	// None found?
	if (furthestNote == nullptr) { return; }


	float timingDisparity = Maths::Abs(furthestFallTime - Settings::FallTime());

	bool noteRegistered = false;
	float perfectWindow = Settings::PerfectWindow();
	float greatWindow = Settings::GreatWindow();
	float missWindow = Settings::MissWindow();

	bool noteHit = true;

	if (timingDisparity < perfectWindow)
	{
		noteRegistered = true;
		NoteHit(eHit::perfect);
	}
	else if (timingDisparity < greatWindow)
	{
		noteRegistered = true;
		NoteHit(eHit::great);
	}
	else if (timingDisparity < missWindow)
	{
		noteHit = false;
		noteRegistered = true;
		NoteMissed();
	}

	if (noteHit)
	{
		m_receptors[column]->NoteHit();
	}

	if (noteRegistered)
		furthestNote->EndDrop();
}

void GameplayScene::NoteHit(eHit hit)
{
	// Increment combo
	if (hit == eHit::perfect)
		m_score->AddPerfect();
	else if (hit == eHit::great)
		m_score->AddGreat();

	UpdateText(hit);
	m_healthBar->IncreaseHealth(HEALTH_GAIN);
}

void GameplayScene::NoteMissed()
{
	m_score->AddMiss();
	m_healthBar->ReduceHealth(HEALTH_LOSS);
	UpdateText(eHit::miss);
}

void GameplayScene::UpdateText(eHit hit)
{
	// Combo text
	m_comboText->setString(std::to_string(m_score->GetCombo()));
	m_comboText->setPosition(m_comboTextPos.x, m_comboTextPos.y);
	m_comboText->setOrigin((m_comboText->getGlobalBounds().width / 2), (m_comboText->getGlobalBounds().height / 2));

	// Update Hit text
	if (hit == eHit::perfect)
	{
		m_hitText->setString("Perfect!");
		m_hitText->setFillColor(sf::Color::Yellow);
	}
	else if (hit == eHit::great)
	{
		m_hitText->setString("Great!");
		m_hitText->setFillColor(sf::Color::Green);
	}
	else
	{
		m_hitText->setString("Miss!");
		m_hitText->setFillColor(sf::Color::Red);
	}
	m_hitText->setPosition(m_hitTextPos.x, m_hitTextPos.y);
	m_hitText->setOrigin((m_hitText->getGlobalBounds().width / 2), (m_hitText->getGlobalBounds().height / 2));

	// Set text related vars
	m_expandCombo = true;
	m_elapsedComboExpandTime = 0.0f;
}

void GameplayScene::TogglePause()
{
	// Pause
	if (!m_paused)
	{
		std::cout << "Paused." << std::endl;
		m_paused = true;
		m_songMusic->pause();
		m_pauseMenu->SetActive(true);
	}
	// Unpause
	else
	{
		std::cout << "Unpaused." << std::endl;
		m_paused = false;
		m_songMusic->play();
		m_pauseMenu->SetActive(false);
	}
}

void GameplayScene::UpdatePaused()
{
	if (Input::Enter.m_keyPressed)
	{
		switch (m_pauseMenu->CurrentOption())
		{
		case ePauseOptions::Resume:
			TogglePause();
			break;

		case ePauseOptions::Restart:
			// Re-load into the current scene. 
			GameManager::ReloadScene();
			break;

		case ePauseOptions::Quit:
			LeaveScene(eLeaveSongReason::quit);
			break;
		}
	}
}

void GameplayScene::LeaveScene(eLeaveSongReason reason)
{
	// Set vars
	m_leaveReason = reason;
	m_songPlaying = false;

	// Stop the music
	m_songMusic->stop();

	// User quit
	if (reason == eLeaveSongReason::quit)
	{
		// Song Select
		GameManager::ChangeScene(eScenes::songSelect);
	}
	// Results (fail or pass)
	if (reason == eLeaveSongReason::dead || reason == eLeaveSongReason::songEnd)
	{
		if (reason == eLeaveSongReason::dead)
		{
			m_score->SetFailed();
		}
		
		ResultsSceneChangeArgs* args = new ResultsSceneChangeArgs();
		args->chosenSongIndex = m_songIndex;
		args->chosenDifficultyIndex = m_diffIndex;
		args->perfectNotes = m_score->GetPerfectNotes();
		args->greatNotes = m_score->GetGreatNotes();
		args->missNotes = m_score->GetMissNotes();
		args->combo = m_score->GetCombo();
		args->maxCombo = m_score->GetMaxCombo();
		args->grade = m_score->CalculateGrade();
		GameManager::ChangeScene(eScenes::resultsScreen, args);
	}
}


float GameplayScene::GetXForColumn(int col)
{
	int temp_sz = 32;

	switch (col)
	{
	case 0:
		return Settings::WindowX() * 0.35f  - temp_sz;

	case 1:
		return Settings::WindowX() * 0.45f - temp_sz;

	case 2:
		return Settings::WindowX() * 0.55f - temp_sz;

	case 3:
		return Settings::WindowX() * 0.65f - temp_sz;
	}
}