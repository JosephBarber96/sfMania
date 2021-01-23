#pragma once

#include <vector>

#include "Scene.h"

class SongInfoBox;
class SongPanel;
class BeatmapDifficultyPanel;
class DifficultySelectMenu;

enum eScrollDir
{
	back,
	forward
};

enum eSelectMode
{
	songSelectMode,
	difficultySelectMode
};

class SongSelectScene : public Scene
{
public:
	SongSelectScene();
	~SongSelectScene();

	// Scene
	void InitScene(SceneChangeArgs* args);
	void UnloadScene();

	void UpdateScene();
	void UpdateSceneTransition(float normalized);
	void RenderScene(sf::RenderWindow* window);

	// Scrolling
	const float SCROLL_TIME = 0.15f;
	const int PANEL_HEIGHT = 50;
	const int PANEL_PADDING = 10;
	const float TIME_BEFORE_SONG = 0.15f;

private:
	// Displays
	std::vector<SongPanel*> m_songPanels;
	std::vector<BeatmapDifficultyPanel*> m_difficultyPanels;
	SongInfoBox* m_songInfoBox;
	sf::Sprite* m_backgroundSprite;
	sf::Texture* m_bannerTexture;
	sf::Sprite* m_bannerSprite;

	// Song
	Song* m_currentSong;
	int m_currentSongIndex;
	int m_targetScrollIndex;	// Index we will be at after scrolling

	// Difficulty
	int m_currentDifficultyIndex;
	DifficultySelectMenu* m_diffSelectMenu;

	// Mode 
	eSelectMode m_mode;

	// Scrolling
	bool m_scrolling;
	float m_elapsedScrollTime;
	float m_timeOnSelectedSong;
	bool m_songPlaying;

	void Scroll(eScrollDir dir);
	void OnScrollFinished();
	void LoadInfoForSong();
	void SetMode(eSelectMode mode);

	// Static const
	static const int BANNER_WIDTH = 400;
	static const int SONG_INFOBOX_HEIGHT = 150;
	static const int SONG_PANEL_WIDTH = 400;
	static const int SONG_PANEL_HEIGHT = 100;
};