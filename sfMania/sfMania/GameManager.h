#pragma once
#include <time.h>

#include <SFML/Graphics.hpp>

enum eScenes;
class Scene;
class Song;
struct SceneChangeArgs;

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
	static void ChangeScene(eScenes p_newScene);
	static void ChangeScene(eScenes p_newScene, SceneChangeArgs* p_args);
	static void ReloadScene();

	// Getters
	static float DeltaTime() { return m_deltaTime; }

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
	void DoSceneChangeInternal(eScenes p_newScene, bool reload = false);

	// Update scene changing
	void UpdateSceneTransition();
	void OnSceneChanged();

	// Update graphics settings
	void ChangeWindowResolution(int x, int y);
	void SetFullscreen(bool fullscreen);

	// Game-State
	Scene* m_currentScene;

	// Scene changing
	SceneChangeArgs* m_sceneChangeArgs;
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