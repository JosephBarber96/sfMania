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

class Scene
{
public:
	Scene();
	~Scene();

	virtual void InitScene() = 0;
	virtual void UnloadScene() = 0;

	virtual void UpdateScene() = 0;
	virtual void RenderScene(sf::RenderWindow * window) = 0;
};