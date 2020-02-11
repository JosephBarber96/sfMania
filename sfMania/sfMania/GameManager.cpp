#include <iostream>
#include <string>

#include "GameManager.h"
#include "Input.h"
#include "GameObject.h"
#include "Song.h"
#include "Settings.h"

#include "ResourceManager.h"
#include "AudioManager.h"

// Scenes
#include "MainMenuScene.h"
#include "SettingsScene.h"
#include "SongSelectScene.h"
#include "GameplayScene.h"
#include "ResultsScene.h"

// Statics
float GameManager::m_deltaTime;
GameManager* GameManager::instance;

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}


void GameManager::Init()
{
	// Init internal components
	instance = this;
	m_clock = new sf::Clock();
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "sf_mania");
	Settings::SetDefaultOnStartup(eResolution::x800y600, 800, 600, false);

	// Init external components and managers
	GameObject::Init();
	ResourceManager::Init();
	AudioManager::Init();
	Input::Init();

	// Render "Now Loading" text
	sf::Text text = sf::Text();
	text.setFont(*ResourceManager::GetFont(eFont::small));
	text.setString("Loading songs...");
	text.setPosition((Settings::WindowX() / 2) - text.getGlobalBounds().width / 2, 
		(Settings::WindowY() / 2) - text.getGlobalBounds().height/2);
	m_window->draw(text);
	m_window->display();

	// Load songs
	Song::LoadAllSongs();

	// Init scene transition
	m_sceneTransitionBlack = sf::RectangleShape();
	m_sceneTransitionBlack.setFillColor(sf::Color::Black);
	m_sceneTransitionBlack.setSize(sf::Vector2f(1920, 1080));

	// Go to Main Menu scene
	m_nextScene = eScenes::mainMenu;
	OnSceneChanged();
}




//---------------------------------------------
// Game loop
//---------------------------------------------

void GameManager::Run()
{
	while (m_window->isOpen())
	{
		// Poll for input events
		UpdateInput();

		// Update
		UpdateObjects();

		// Render
		RenderObjects();

		// Calculate delta time
		m_deltaTime = m_clock->restart().asSeconds();
	}	
}


void GameManager::UpdateInput()
{
	Input::UpdateInput(m_window);
}


void GameManager::UpdateObjects()
{
	// Run GameManager logic (e.g. scene changing)
	SelfUpdate();

	// Run Scene logic
	m_currentScene->UpdateScene();

	// Run GameObject update loops
	for (GameObject* obj : GameObject::GameObjects)
	{
		if (obj->IsActive())
		{
			obj->Update();
		}
	}
}


void GameManager::RenderObjects()
{
	// Clear window
	m_window->clear();

	// Pass window into the scene so it can handle rendering the objects in the active scene
	m_currentScene->RenderScene(m_window);

	// Render any GameManager stuff (e.g. scene transition)
	SelfRender();

	// Display
	m_window->display();
}




//---------------------------------------------
// Self Update + Render
//---------------------------------------------

void GameManager::SelfUpdate()
{
	if (m_changingScene)
		UpdateSceneTransition();
}


void GameManager::SelfRender()
{
	if (!m_changingScene) { return; }

	// Scene transition
	if (m_changingScene)
	{
		// Calc normalized time
		float normalized = m_elapsedSceneTransitionTime / TRANS_TIME;

		// Get alpha
		sf::Color col = m_sceneTransitionBlack.getFillColor();
		float a = 0;

		// Fade in
		if (!m_sceneChanged)
			a = 255 * normalized;
		// Fade out
		else
			a = (255 * (1 - normalized));

		// Set alpha
		col.a = a;
		m_sceneTransitionBlack.setFillColor(col);

		m_window->draw(m_sceneTransitionBlack);
	}
}




//---------------------------------------------
// Setting currently selected song
//---------------------------------------------

void GameManager::SetChosenSong(Song* song, int difficulty)
{
	instance->m_currentlyChosenSong = song;
	instance->m_currentlyChosenDifficulty = difficulty;
}




//---------------------------------------------
// Change scene
//---------------------------------------------

void GameManager::ChangeScene(eScenes p_newScene)
{
	instance->SceneChange(p_newScene);
}

void GameManager::ReloadScene()
{
	instance->SceneChange(instance->m_scene, true);
}

void GameManager::SceneChange(eScenes p_newScene, bool reload)
{
	if (m_changingScene) { return; }
	if (m_scene == p_newScene && !reload) { return; }

	m_changingScene = true;
	m_sceneChanged = false;
	m_elapsedSceneTransitionTime = 0.0f;
	m_nextScene = p_newScene;
}

void GameManager::UpdateSceneTransition()
{
	m_elapsedSceneTransitionTime += DeltaTime();
	float normalized = m_elapsedSceneTransitionTime / TRANS_TIME;

	// Mid transition - change scene
	if (normalized >= 1.0f && !m_sceneChanged)
	{
		OnSceneChanged();
	}

	// End transition
	if (normalized >= 2.0f)
	{
		m_changingScene = false;
	}

	float normalizedPerHalfTransition = normalized;
	if (normalizedPerHalfTransition > 1.0f)
		normalizedPerHalfTransition -= 1.0f;
	m_currentScene->UpdateSceneTransition(normalizedPerHalfTransition);
}

void GameManager::OnSceneChanged()
{
	m_sceneChanged = true;

	// Unload and delete current scene
	if (m_currentScene != nullptr)
	{
		m_currentScene->UnloadScene();
		delete m_currentScene;
	}

	// Load new scene
	m_scene = m_nextScene;
	m_nextScene = eScenes::none;	
	switch (m_scene)
	{
	case eScenes::mainMenu:
		m_currentScene = new MainMenuScene();
		break;

	case eScenes::options:
		m_currentScene = new SettingsScene();
		break;
		
	case eScenes::songSelect:
		m_currentScene = new SongSelectScene();
		break;

	case eScenes::playingSong:
		m_currentScene = new GameplayScene();
		break;

	case eScenes::resultsScreen:
		m_currentScene = new ResultsScene();
		break;
	}
	m_currentScene->InitScene();

	std::cout << "Now in scene: " << m_scene << std::endl;
}



//---------------------------------------------
// Update graphics settings
//---------------------------------------------

void GameManager::ChangeWindowResolution(int x, int y)
{
	m_window->create(sf::VideoMode(Settings::WindowX(), Settings::WindowY()), "sf_mania", sf::Style::Default);
}

void GameManager::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
		m_window->create(sf::VideoMode(Settings::WindowX(), Settings::WindowY()), "sf_mania", sf::Style::Fullscreen);
	else
		m_window->create(sf::VideoMode(Settings::WindowX(), Settings::WindowY()), "sf_mania", sf::Style::Default);
}