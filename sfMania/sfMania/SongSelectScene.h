#pragma once

#include <vector>

#include "Scene.h"

class SongInfoBox;
class SongPanel;
class BeatmapDifficultyPanel;

enum eScrollDir
{
	up,
	down
};

class SongSelectScene : public Scene
{
public:
	SongSelectScene();
	~SongSelectScene();

	// Scene
	void InitScene();
	void UnloadScene();
	void UpdateScene();
	void RenderScene(sf::RenderWindow* window);

	// Scrolling
	const float SCROLL_TIME = 0.15f;
	const int PANEL_HEIGHT = 75;
	const int PANEL_PADDING = 15;

private:
	// Displays
	std::vector<SongPanel*> m_songPanels;
	std::vector<BeatmapDifficultyPanel*> m_difficultyPanels;
	SongInfoBox* m_songInfoBox;
	sf::Texture* m_backgroundTexture;
	sf::Sprite* m_backgroundSprite;
	sf::Texture* m_bannerTexture;
	sf::Sprite* m_bannerSprite;

	// Song
	int m_currentSongIndex;
	int m_targetScrollIndex;	// Index we will be at after scrolling
	int m_currentDifficultyIndex;

	// Scrolling
	bool m_scrolling;
	float m_elapsedScrollTime;

	void Scroll(eScrollDir dir);
	void OnScrollFinished();
	void LoadInfoForSong();

	// Static const
	static const int BANNER_WIDTH = 400;
	static const int SONG_INFOBOX_HEIGHT = 205;
	static const int SONG_PANEL_WIDTH = 400;
	static const int SONG_PANEL_HEIGHT = 100;
};