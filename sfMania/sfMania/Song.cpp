#include <string>

#include "Song.h"
#include "Utility.h"
#include "StepMap.h"
#include "BPM.h"

std::vector<Song*> Song::AllSongs;

Song::Song()
{
	m_stepMaps = std::vector<StepMap*>();
}

Song::~Song()
{
}


void Song::LoadAllSongs()
{
	std::string dir = ".\\Assets\\Songs";

	// Get all of the directories for every .sm file
	std::vector<std::string> songList = Utility::FindAllSmFiles(dir);

	// Load each song
	Song::AllSongs = std::vector<Song*>();
	for (std::string filename : songList)
	{
		Song* song = Utility::LoadSongFromFile(filename);
		AllSongs.push_back(song);
	}
}


// Adding info

void Song::AddStepmap(StepMap* stepMap)
{
	m_stepMaps.push_back(stepMap);
}

void Song::AddBpm(BPM* bpm)
{
	m_BPMs.push_back(bpm);
}



// Reading info

int Song::StepmapCount() const
{
	return m_stepMaps.size();
}

StepMap* Song::GetStepmap(int index) const
{
	return m_stepMaps[index];
}

BPM* Song::GetBpmForBeat(int beat) const
{
	for (int i = 0; i < m_BPMs.size(); i++)
	{
		// Last BPM
		if (i >= m_BPMs.size()-1)
			return m_BPMs[i];

		// Beat >= currentBPM and < nextBPM
		if (beat >= m_BPMs[i]->Beat() && beat < m_BPMs[i + 1]->Beat())
			return m_BPMs[i];
	}
}

float Song::GetMinBpm() const
{
	float minBPM = 10000;

	for (int i = 0; i < m_BPMs.size(); i++)
	{
		if (m_BPMs[i]->Bpm() < minBPM)
			minBPM = m_BPMs[i]->Bpm();
	}

	return minBPM;
}

float Song::GetMaxBpm() const
{
	float maxBpm = 0;

	for (int i = 0; i < m_BPMs.size(); i++)
	{
		if (m_BPMs[i]->Bpm() > maxBpm)
			maxBpm = m_BPMs[i]->Bpm();
	}

	return maxBpm;
}