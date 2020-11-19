#pragma once
#include <thread>
#include <chrono>
#include <vector>
#include <array>

#include <SFML/Audio.hpp>

#include "Scene.h"

class Song;
class Score;
class Receptor;
class Note;
class LongNote;
class StepMap;
class Measure;
class PauseMenu;
class HealthBar;
class ProgressBar;
class Animation;
enum eHit;

enum eLeaveSongReason
{
	quit,
	songEnd,
	dead,
};

class GameplayScene : public Scene
{
public:
	GameplayScene();
	~GameplayScene();

	const int MAX_HEALTH = 100;
	const int HEALTH_LOSS = 10;
	const int HEALTH_GAIN = 1;

	// Scene
	void InitScene();
	void UnloadScene();
	void UpdateScene();
	void UpdateSceneTransition(float normalized);
	void RenderScene(sf::RenderWindow * window);

	// GameplayScene
	void AlertNoteMissed();
	bool SongPaused() const { return m_paused; }
	

private:

	// GameplayScene methods
	//
	// Update functions
	void UpdateReceptorInput();
	void UpdateSongAndNotes();
	void UpdateComboText();
	// Other	
	void Play();
	void DropNote(int column);
	LongNote* DropLongNote(int column);
	void CheckForHit(int column);
	void NoteHit(eHit hit);
	void NoteMissed();
	void UpdateText(eHit hit);
	void TogglePause();
	void UpdatePaused();
	void LeaveScene(eLeaveSongReason reason);

	static float GetXForColumn(int col);

	// State logic
	bool m_paused;

	// Song components
	Song* m_currentSong;
	sf::Music* m_songMusic;

	// Threads
	std::thread m_noteThread;
	std::thread m_musicThread;

	// Song logic
	bool m_songPlaying;
	bool m_songStarted;
	bool m_init;
	bool m_endOfNotes;
	//
	int m_curMeasureIndex;
	int m_curLineIndex;
	Measure* m_curMeasure;
	int m_currentBeat;
	//
	int m_curBeatLine;
	int m_linesPerBeat;
	float m_currentBpm;
	//
	float m_initialNoteDelay;
	float m_initialSongDelay;
	float m_nextLineDelay;
	//
	float m_elapsedSongTime;
	float m_nextNoteDropTime;
	//
	StepMap* m_stepMap;
	eLeaveSongReason m_leaveReason;
	
	// Gameplay vars
	Score* m_score;
	HealthBar* m_healthBar;
	ProgressBar* m_progressBar;
	// Receptors
	std::array<Receptor*, 4> m_receptors;

	// Notes
	int m_currentNoteIndex;
	std::vector<Note*> m_notes;
	int m_currentLongnoteIndex;
	std::vector<LongNote*> m_longNotes;
	std::array<LongNote*, 4> m_activeLongNotes;

	// UI
	sf::Texture * m_backgroundTexture;
	sf::Sprite * m_backgroundSprite;
	sf::RectangleShape * m_playBox;

	PauseMenu* m_pauseMenu;

	// Combo score
	sf::Text * m_comboText;
	sf::Vector2i m_comboTextPos;
	sf::Text * m_hitText;
	sf::Vector2i m_hitTextPos;
	bool m_expandCombo;
	float m_elapsedComboExpandTime;
	float m_targetComboExpandTime;
};

