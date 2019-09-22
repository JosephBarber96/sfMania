#include <iostream>
#include <string>

#include "GameplayScene.h"

#include "GameManager.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "Settings.h"
#include "ResultsScene.h"
#include "Input.h"
#include "Maths.h"

#include "Song.h"
#include "StepMap.h"
#include "Measure.h"
#include "Line.h"
#include "BPM.h"
#include "Score.h"

#include "Receptor.h"
#include "Note.h"
#include "LongNote.h"


GameplayScene::GameplayScene()
{
	m_notes = std::vector<Note*>();
	m_longNotes = std::vector<LongNote*>();
	m_activeLongNotes = std::array<LongNote*, 4>();
}


GameplayScene::~GameplayScene()
{
}




//--------------------------
// Scene
//--------------------------

void GameplayScene::InitScene()
{
	// Init vars
	m_score = new Score();

	// Get song
	m_currentSong = GameManager::GetCurrentSong();

	// Init receptors
	m_receptorLeft = new Receptor();
	m_receptorLeft->InitSelf(0, GetXForColumn(0));
	m_receptorLeftMid = new Receptor();
	m_receptorLeftMid->InitSelf(1, GetXForColumn(1));
	m_receptorRightMid = new Receptor();
	m_receptorRightMid->InitSelf(2, GetXForColumn(2));
	m_receptorRight = new Receptor();
	m_receptorRight->InitSelf(3, GetXForColumn(3));

	// Init music
	m_songMusic = new sf::Music();

	// Init notes
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
		m_comboText->setFont(*ResourceManager::GetFont(eFont::bold));
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
		m_hitText->setFont(*ResourceManager::GetFont(eFont::bold));
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


	m_musicThread = std::thread(&GameplayScene::Play, this);
}

void GameplayScene::UnloadScene()
{
	// Delete heap-allocated notes
	int sz = m_notes.size();
	for (int i = sz-1; i > 0; i--)
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
	delete m_backgroundTexture;
	delete m_backgroundSprite;
	delete m_playBox;
	delete m_comboText;
	if (m_leaveReason == eLeaveSongReason::quit)
		delete m_score;
}

void GameplayScene::UpdateScene()
{
	// Receptor pressed
	if (Input::W.m_keyPressed)
	{
		CheckForHit(0);
		m_receptorLeft->Pressed();
	}
	if (Input::E.m_keyPressed)
	{
		CheckForHit(1);
		m_receptorLeftMid->Pressed();
	}
	if (Input::I.m_keyPressed)
	{
		CheckForHit(2);
		m_receptorRightMid->Pressed();
	}	
	if (Input::O.m_keyPressed)
	{
		CheckForHit(3);
		m_receptorRight->Pressed();
	}
		

	// Receptor released
	if (Input::W.m_keyReleased)
		m_receptorLeft->Released();
	if (Input::E.m_keyReleased)
		m_receptorLeftMid->Released();
	if (Input::I.m_keyReleased)
		m_receptorRightMid->Released();
	if (Input::O.m_keyReleased)
		m_receptorRight->Released();


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

	// Quit
	if (Input::Esc.m_keyPressed)
		LeaveScene(eLeaveSongReason::songEnd);
}

void GameplayScene::RenderScene(sf::RenderWindow * window)
{
	// Background
	window->draw(*m_backgroundSprite);
	// Playbox
	window->draw(*m_playBox);

	// Receptors
	m_receptorLeft->RenderSelf(window);
	m_receptorLeftMid->RenderSelf(window);
	m_receptorRightMid->RenderSelf(window);
	m_receptorRight->RenderSelf(window);

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

	// Combo text
	window->draw(*m_comboText);
	window->draw(*m_hitText);
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

	// Calculate offset
	float songOffset = m_currentSong->m_offset * 1000;
	float noteOffset = Settings::FallTime() * 1000;
	
	// Case 0: Positive or 0 offset
	if (songOffset >= 0)
	{
		float newOffset = songOffset + noteOffset;

		// Notes
		m_noteThread = std::thread(&GameplayScene::PlayNotes, this);
		m_noteThread.detach();
		// Wait
		std::this_thread::sleep_for(std::chrono::milliseconds(long long(newOffset)));
		// Song
		m_songMusic->play();
	}

	// Case 1: 
	// Negative offset BUT noteOffset > abs(songOffset)
	//
	// Therefore: noteOffset - abs(songOffet) > 0
	if (songOffset < 0 && noteOffset > fabsf(songOffset))
	{
		// Instead of having beat0 occur after the song
		// It now occurs before the song
		float newOffset = noteOffset - fabsf(songOffset);

		// Notes
		m_noteThread = std::thread(&GameplayScene::PlayNotes, this);
		m_noteThread.detach();
		// Wait
		std::this_thread::sleep_for(std::chrono::milliseconds(long long(newOffset)));
		// Song
		m_songMusic->play();
	}

	// Case 2:
	// Negative offset BUT noteOffset < abs(songOffset)
	//
	// Therefore: noteOffset - abs(songOffset) < 0
	else if (songOffset < 0 && noteOffset < fabsf(songOffset))
	{
		float newOffset = (fabsf(songOffset)) - noteOffset;

		// Song
		m_songMusic->play();
		// Wait
		std::this_thread::sleep_for(std::chrono::milliseconds(long long(newOffset)));
		// Notes
		m_noteThread = std::thread(&GameplayScene::PlayNotes, this);
		m_noteThread.detach();
	}

	// Init playing
	m_terminateSong = false;
}

void GameplayScene::AlertNoteMissed()
{
	NoteMissed();
}


// Private

void GameplayScene::PlayNotes()
{
	StepMap* stepmap = m_currentSong->GetStepmap(0);

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

	int currentBeat = 0;

	float current_bpm = -1;
	float current_wait = -1;

	for (const Measure* measure : stepmap->GetMeasures())
	{
		// Every {linesPerBeat} number of lines, we count beatCount++;
		int lineCount = 0;
		int numberOfLines = measure->LineCount();
		int linesPerBeat = numberOfLines / 4;	

		for (const Line* line : measure->GetLines())
		{
			// Count beats
			if (lineCount = linesPerBeat - 1)
			{
				lineCount = 0;
				currentBeat++;
			}
			lineCount++;

			// For each column of the line
			for (int i = 0; i < 4; i++)
			{
				if (m_terminateSong) { return; }				

				// Handle Note
				switch (line->GetChar(i))
				{
					// Regular arrow
				case '1':
					DropNote(i);
					break;

					// Long head
				case '2':
					// For now, treat rolls as long notes
				case '4':
					m_activeLongNotes[i] = DropLongNote(i);
					break;

					// Long tail
				case '3':
					m_activeLongNotes[i]->EndTail();
					break;
				}
			}

			// Calculate how long we have to wait { at the current BPM and Measure }
			// untill we process the next line
			{
				// BPM of current beat
				float bpm = m_currentSong->GetBpmForBeat(currentBeat)->Bpm();
				if (current_bpm != bpm)
				{
					current_bpm = bpm;
					std::cout << "Switching to " << current_bpm << " bpm!" << std::endl;
				}

				// Beats per sec
				float current_bps = current_bpm / 60;

				// time in ms
				float seconds_in_ms = 1000;

				// how long between each hit
				float current_wait_time = seconds_in_ms / current_bps;

				// hits per metrinome
				float hits_per_bpm = linesPerBeat;

				// wait pet beat
				float wait_time_per_beat = current_wait_time / hits_per_bpm;

				current_wait = wait_time_per_beat;

				// Wait
				std::this_thread::sleep_for(std::chrono::milliseconds(long long(current_wait)));
			}
		}
	}

	std::cout << "Song done!" << std::endl;
	LeaveScene(eLeaveSongReason::songEnd);

	// std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
		noteRegistered = true;
		NoteMissed();
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
}

void GameplayScene::NoteMissed()
{
	m_score->AddMiss();
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


void GameplayScene::LeaveScene(eLeaveSongReason reason)
{
	// Set vars
	m_leaveReason = reason;
	m_terminateSong = true;

	// Stop the music
	m_songMusic->stop();

	// User quit
	if (reason == eLeaveSongReason::quit)
	{
		// Song Select
		GameManager::ChangeScene(eScenes::songSelect);
	}
	else if (reason == eLeaveSongReason::songEnd)
	{
		// Score screen
		ResultsScene::LoadSceneResults(m_score);
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