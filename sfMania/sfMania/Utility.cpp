#include "Utility.h"

// C++ 17 File System
#include <filesystem>
#include <experimental/filesystem>

#include <sstream>
#include <fstream>

#include "Song.h"
#include "StepMap.h"
#include "Measure.h"
#include "BPM.h"
#include "Settings.h"
#include "ResourceManager.h"

namespace Utility
{
	//------------------------------------------
	// Generic util
	//------------------------------------------

	std::vector<std::string> ExplodeString(std::string line, char explode)
	{
		std::vector<std::string> ret;

		std::string str = "";
		for (char c : line)
		{
			if (c == explode)
			{
				ret.push_back(str);
				str = "";
				continue;
			}
			str += c;
		}
		ret.push_back(str);
		return ret;
	}




	//------------------------------------------
	// SFML Util
	//------------------------------------------

	sf::Vector2f GetScaleForTargetSize(sf::Texture * tex, float targetWidth, float targetHeight)
	{
		float width = tex->getSize().x;
		float height = tex->getSize().y;

		float widthScale = (targetWidth / width);
		float heightScale = (targetHeight / height);

		return (sf::Vector2f(widthScale, heightScale));
	}

	float GetXForText(sf::Text* text, float normalizedX)
	{
		return ((Settings::WindowX() * normalizedX) - (text->getGlobalBounds().width * 0.5f));
	}

	sf::Vector2f NormalizedToScreen(sf::Text* text, float normalizedX, float normalizedY)
	{
		float half_width = text->getGlobalBounds().width * 0.5f;
		float half_height = text->getGlobalBounds().height * 0.5f;

		float x = Settings::WindowX() * normalizedX;
		float y = Settings::WindowY() * normalizedY;

		return sf::Vector2f(x - half_width, y - half_height);
	}

	void SetupText(sf::Text* text, eFont font, std::string textString, int size, sf::Color fillColour, sf::Color outlineColour, int outlineThickness, int x, int y)
	{
		SetupText(text, ResourceManager::GetFont(font), textString, size, fillColour, outlineColour, outlineThickness, x, y);
	}

	void SetupText(sf::Text* text, sf::Font* font, std::string textString, int size, sf::Color fillColour, sf::Color outlineColour, int outlineThickness, int x, int y)
	{
		text->setFont(*font);
		text->setString(textString);
		text->setCharacterSize(size);
		text->setOutlineColor(outlineColour);
		text->setOutlineThickness(outlineThickness);
		text->setFillColor(fillColour);
		text->setPosition(x, y);
	}

	sf::Color UnhighlightedColour() { return sf::Color(0, 0, 0, 225); }
	sf::Color HighlightedColour() { return sf::Color(255, 255, 255, 128); }




	//------------------------------------------
	// SFMania Util
	//------------------------------------------

	std::string GetStringForResolution(eResolution res)
	{
		std::string str = "";

		switch (res)
		{
		case eResolution::x800y600:
			str = "800 x 600";
			break;

		case eResolution::x1024y786:
			str = "1024 x 768";
			break;

		case eResolution::x1280y720:
			str = "1280 x 720";
			break;

		case eResolution::x1280y960:
			str = "1280 x 960";
			break;

		case eResolution::x1366y768:
			str = "1366 x 768";
			break;

		case eResolution::x1600y900:
			str = "1600 x 900";
			break;

		case eResolution::x1920y1080:
			str = "1920 x 1080";
			break;
		}

		return str;
	}




	//------------------------------------------
	// Stepmania .sm file Util
	//------------------------------------------

	std::vector<std::string> FindAllSmFiles(std::string directory)
	{
		std::vector<std::string> smFiles;

		// filesystem::recursive_directory_iterator = search all sub folders
		// filesystem::directory_iterator = search only given folder

		auto files = std::experimental::filesystem::recursive_directory_iterator(directory);
		for (auto &file : files)
		{
			// Use a stringstream to get the string value of the file object
			std::stringstream ss;
			ss << file;

			// Explode by a . to get the filename
			std::vector<std::string> pieces = Utility::ExplodeString(ss.str(), '.');

			// If its a .sm file, we want it
			if (pieces[pieces.size() - 1] == "sm")
			{
				smFiles.push_back(ss.str());
			}
		}

		return smFiles;
	}

	Song* LoadSongFromFile(std::string directory)
	{
		// Reading in the step file
		std::string line;
		std::ifstream reader(directory);

		Song* song = new Song();
		StepMap* stepMap = new StepMap();
		Measure* measure = new Measure();

		bool readingNoteData = 0;
		int notesMetadataCounter = 0;

		if (reader.is_open())
		{
			// Read each line
			while (getline(reader, line))
			{
				// Reading song metadata
				if (!readingNoteData && line[0] == '#')
				{
					// #TITLE
					if (line.find("#TITLE:") != std::string::npos)
						song->m_title = HandleTitle(line);

					//#ARTIST
					else if (line.find("#ARTIST:") != std::string::npos)
						song->m_artist = HandleArtist(line);

					//#BPM
					else if (line.find("#BPMS:") != std::string::npos)
						HandleBpm(song, line);

					//#MUSIC
					else if (line.find("#MUSIC:") != std::string::npos)
						song->m_musicFile = HandleMusic(line, directory);

					//#OFFSET
					else if (line.find("#OFFSET:") != std::string::npos)
						song->m_offset = HandleOffset(line);

					//#BANNER
					else if (line.find("#BANNER:") != std::string::npos)
						song->m_bannerFile = HandleBanner(line, directory);

					//#BACKGROUND
					else if (line.find("#BACKGROUND:") != std::string::npos)
						song->m_backgroundFile = HandleBackground(line, directory);

					//#NOTES
					else if (line.find("#NOTES:") != std::string::npos)
					{
						// Set flags to begin reading note data and then skip this line
						readingNoteData = true;
						notesMetadataCounter = 0;
						stepMap = new StepMap();
						continue;
					}
				}

				// Reading note data
				else if (readingNoteData)
				{
					// Reading in measures
					if (notesMetadataCounter >= 5)
					{
						// If we find a "," it's the end of the measure
						if (line.find(",") != std::string::npos)
						{
							// Add measure to the stepMap
							stepMap->AddMeasure(measure);

							// Create a new measure
							measure = new Measure();
						}

						// If we find a ";" it's the end of the current stepmap
						else if (line.find(";") != std::string::npos)
						{
							// Add the measure to the stepMap
							stepMap->AddMeasure(measure);

							// Add the stepmap to the song
							song->AddStepmap(stepMap);

							// Stop reading noteData...
							// This sets the flag to look for the next #NOTES
							readingNoteData = false;
						}

						// Else, add the notes to the current measure
						else
						{
							measure->AddLine(line);
						}
					}
					// Reading in beatmap metadata
					else
					{
						if (line == "") continue;
						// Grab the line and remove the ending ":"
						std::string currentLine = line;
						currentLine.pop_back();

						switch (notesMetadataCounter)
						{
							// Chart type
						case 0:
							stepMap->m_chartType = currentLine;
							break;

							// Description
						case 1:
							stepMap->m_description = currentLine;
							break;

							// Difficulty name
						case 2:
							stepMap->m_difficultyName = currentLine;
							while (stepMap->m_difficultyName[0] == ' ')
								stepMap->m_difficultyName.erase(stepMap->m_difficultyName.begin());
							break;

							// Numerical difficulty
						case 3:
							stepMap->m_numericalDifficulty = std::stoi(currentLine);
							break;

							// Groove data (TODO)
						case 4:
							break;
						}

						notesMetadataCounter++;
					}
				}
			}
			reader.close();
		}	

		return song;
	}

	std::string HandleTitle(std::string line)
	{
		/* Example line:
		#TITLE:song_name;
		*/

		// Explode by : to get song_name;
		std::vector<std::string> exploded = ExplodeString(line, ':');

		// Explode by ; to get song_name
		std::vector<std::string> song_name = ExplodeString(exploded[exploded.size() - 1], ';');

		// Assign title
		return song_name[0];
	}

	std::string HandleArtist(std::string line)
	{
		/* 
		Example line:
		#ARTIST:artist_name;
		*/

		// Explode by : to get artist_name;
		std::vector<std::string> exploded = ExplodeString(line, ':');

		// Explode by ; to get artist_name;
		std::vector<std::string> artist_name = ExplodeString(exploded[exploded.size() - 1], ';');

		// Assign artist
		return artist_name[0];
	}

	std::string HandleMusic(std::string line, std::string fileDirectory)
	{
		/*
		Example line:
		#MUSIC:0kash - Little princess has no identity.ogg;
		*/

		// Explode the line by : to get the filename with the ;
		std::vector<std::string> exploded = Utility::ExplodeString(line, ':');

		// Get the filename of the .ogg
		std::string musicFile = exploded[1];

		// Remove the ending ;
		musicFile.pop_back();

		// Grab the diretory
		std::string directory = fileDirectory;
		while (directory[directory.size() - 1] != '\\')
		{
			directory.pop_back();
		}

		directory.append(musicFile);

		// Return
		return directory;
	}

	void HandleBpm(Song* song, std::string line)
	{
		/*
		Example line:
		#BPMS:0.000=192.000,260.000=96.000,275.000=192.000;
		*/

		// Explode the line by : to get the number values
		std::vector<std::string> numValues = Utility::ExplodeString(line, ':');

		// We now have
		// 0.000=192.000,260.000=96.000,275.000=192.000;

		// Explode by , to go get the beat=bpm pairs
		std::vector<std::string> bpmsExploded = Utility::ExplodeString(numValues[1], ',');

		// Remove the final ; off of the last beat-bpm pair
		bpmsExploded[bpmsExploded.size() - 1].erase(bpmsExploded[bpmsExploded.size() - 1].end() - 1);

		// Create the BPMs
		for (std::string str : bpmsExploded)
		{
			std::vector<std::string> vals = Utility::ExplodeString(str, '=');

			song->AddBpm(new BPM(std::stoi(vals[0]), std::stof(vals[1])));
		}
	}

	std::string HandleBackground(std::string line, std::string fileDirectory)
	{
		/*
		Example line:
		BACKGROUND:background-image.png;
		*/

		// Explode by : to get background-image.png;
		std::vector<std::string> exploded = ExplodeString(line, ':');

		// Filename has been provided
		if (exploded[1] != ";")
		{
			// Explode by ; to get background-image.png;
			std::vector<std::string> background = ExplodeString(exploded[exploded.size() - 1], ';');

			// Grab the filename
			std::string backgroundFileName = background[0];

			// Append it to the correct directory
			std::string directory = fileDirectory;
			while (directory[directory.size() - 1] != '\\')
			{
				directory.pop_back();
			}
			directory.append(backgroundFileName);

			// Return
			return directory;
		}

		// No filename provided
		else
		{
			// Get the directory
			std::string directory = fileDirectory;
			while (directory[directory.size() - 1] != '\\')
			{
				directory.pop_back();
			}
			directory.pop_back();

			// Search the directory for a file containing the substring "bg.png"
			for (auto & file : std::experimental::filesystem::recursive_directory_iterator(directory))
			{
				// Use a stringstream to get the string value of the file object
				std::stringstream ss;
				ss << file;

				// If it contains the substring bg.png, we want it
				if (ss.str().find("bg.png") != std::string::npos)
				{
					// Return
					return ss.str();
				}
			}
			return "";
		}
	}

	std::string HandleBanner(std::string line, std::string fileDirectory)
	{
		/*
		Example line:
		#BANNER:banner-image.png;
		*/

		// Explode by : to get banner-image.png;
		std::vector<std::string> exploded = ExplodeString(line, ':');

		// Filename provided...
		if (exploded[1] != ";")
		{
			// Explode by ; to get banner-image.png
			std::vector<std::string> banner = ExplodeString(exploded[exploded.size() - 1], ';');

			// Grab the file name
			std::string bannerFileName = banner[0];

			// Append it to the correct directory
			std::string directory = fileDirectory;
			while (directory[directory.size() - 1] != '\\')
			{
				directory.pop_back();
			}
			directory.append(bannerFileName);

			// Return 
			return directory;
		}

		// No filename provided...
		else
		{
			// Get the directory
			std::string directory = fileDirectory;

			while (directory[directory.size() - 1] != '\\')
			{
				directory.pop_back();
			}
			directory.pop_back();

			// Search the directory for a file containing the substring "bn.png"
			for (auto & file : std::experimental::filesystem::recursive_directory_iterator(directory))
			{
				// Use a stringstream to get the string value of the file object
				std::stringstream ss;
				ss << file;

				// If it contains the substring bn.png, we want it
				if (ss.str().find("bn.png") != std::string::npos)
				{
					return ss.str();
				}
			}
			return "";
		}
	}

	float HandleOffset(std::string line)
	{
		/*
		Example line:
		#OFFSET:-0.042;
		*/

		// Explode the line by the : to get the offset with a ;
		std::vector<std::string> exploded = Utility::ExplodeString(line, ':');

		// Grab the correct string
		std::string offsetString = exploded[1];

		// Remove the ;
		offsetString.pop_back();

		// convert to float
		float offset = std::stof(offsetString);

		return offset;
	}
}
