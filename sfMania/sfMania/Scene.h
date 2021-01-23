#pragma once

#include <SFML/Graphics.hpp>

enum eScenes
{
	none,
	mainMenu,
	options,
	songSelect,
	playingSong,
	resultsScreen,
};

struct SceneChangeArgs;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void InitScene(SceneChangeArgs* args) = 0;
	virtual void UnloadScene() = 0;

	virtual void UpdateScene() = 0;
	virtual void UpdateSceneTransition(float normalized) = 0;
	virtual void RenderScene(sf::RenderWindow * window) = 0;
};