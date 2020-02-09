#pragma once
#include <time.h>

#include <SFML/Graphics.hpp>

enum eScenes;
class Scene;
class Song;

class GameManager
{
public:
	GameManager();
	~GameManager();

	// Consts
	const float TRANS_TIME = 0.15f;

	// Game init and main loop
	void Init();
	void Run();

	// Static behaviour
	static void SetChosenSong(Song* song, int difficulty) { instance->m_currentlyChosenSong = song; instance->m_currentlyChosenDifficulty = difficulty; }
	static void ChangeScene(eScenes p_newState);

	// Getters
	static float DeltaTime() { return m_deltaTime; }
	static Song* GetCurrentSong() { return instance->m_currentlyChosenSong; }
	static int GetCurrentChosenDifficulty() { return instance->m_currentlyChosenDifficulty; }

private:

	friend class Settings;

	// Private singleton, used when Static methods are called (e.g. ChangeScene)
	static GameManager* instance;
	
	// Update and Render loops
	void UpdateInput();
	void UpdateObjects();
	void RenderObjects();

	// Internal update/render logic
	void SelfUpdate();
	void SelfRender();
	void SceneChange(eScenes p_newScene);

	// Update scene changing
	void UpdateSceneTransition();
	void OnSceneChanged();

	// Update graphics settings
	void ChangeWindowResolution(int x, int y);
	void SetFullscreen(bool fullscreen);

	// Game-State
	Scene* m_currentScene;
	Song* m_currentlyChosenSong;
	int m_currentlyChosenDifficulty;

	// Scene changing
	eScenes m_scene;
	eScenes m_nextScene;
	bool m_changingScene;
	bool m_sceneChanged;
	float m_elapsedSceneTransitionTime;
	sf::RectangleShape m_sceneTransitionBlack;

	// Internal components
	sf::RenderWindow * m_window;
	sf::Clock * m_clock;
	static float m_deltaTime;
};