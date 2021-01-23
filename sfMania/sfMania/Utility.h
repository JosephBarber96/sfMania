#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

class Song;
enum eResolution;
enum eFont;

namespace Utility
{
	// Generic util
	std::vector<std::string> ExplodeString(std::string line, char explode);

	// SFML Util
	sf::Vector2f GetScaleForTargetSize(sf::Texture * tex, float targetWidth, float targetHeight);
	sf::Vector2f NormalizedToScreen(sf::Text* text, float normalizedX, float normalizedY);
	float GetXForText(sf::Text* text, float x);
	void SetupText(sf::Text* text, eFont font, std::string textString, int size, sf::Color fillColour, sf::Color outlineColour, int outlineThickness, int x = 0, int y = 0);
	void SetupText(sf::Text* text, sf::Font* font, std::string textString, int size, sf::Color fillColour, sf::Color outlineColour, int outlineThickness, int x = 0, int y = 0);
	sf::Color UnhighlightedColour();
	sf::Color HighlightedColour();
	sf::Color Lerp(sf::Color a, sf::Color b, float t);

	// SFMania util
	std::string GetStringForResolution(eResolution res);

	// Stepmania .sm file util
	std::vector<std::string> FindAllSmFiles(std::string directory);
	Song* LoadSongFromFile(std::string directory);
	std::string HandleTitle(std::string line);
	std::string HandleArtist(std::string line);
	std::string HandleMusic(std::string line, std::string fileDirectory);
	void HandleBpm(Song* song, std::string line);
	std::string HandleBackground(std::string line, std::string fileDirectory);
	std::string HandleBanner(std::string line, std::string fileDirectory);
	float HandleOffset(std::string line);
	sf::Color GetDifficultyColour(int difIndex);
};