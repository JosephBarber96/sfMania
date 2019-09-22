#pragma once

#include <vector>
#include <string>

class StepMap;
class BPM;

class Song
{
public:
	Song();
	~Song();

	std::string m_title;
	std::string m_artist;
	std::string m_musicFile;
	float m_offset;

	std::string m_backgroundFile;
	std::string m_bannerFile;

	static Song* GetSong(int i) { return AllSongs[i]; }
	static int SongCount() { return AllSongs.size(); }
	static void LoadAllSongs();

	// Adding info
	void AddStepmap(StepMap* stepMap);
	void AddBpm(BPM* bpm);

	// Getting Info
	StepMap* GetStepmap(int difficulty) const;
	BPM* GetBpmForBeat(int beat) const;
	float GetMinBpm() const;
	float GetMaxBpm() const;

private:
	static std::vector<Song*> AllSongs;


	std::vector<StepMap*> m_stepMaps;
	std::vector<BPM*> m_BPMs;
};

