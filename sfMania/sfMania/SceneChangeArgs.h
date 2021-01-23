#pragma once

enum eGrade;

class Score; 

//Used to pass information through scene transitions
struct SceneChangeArgs
{

};

struct SongSceneChangeArgs : SceneChangeArgs
{
	int chosenSongIndex;
	int chosenDifficultyIndex;
};

struct ResultsSceneChangeArgs : SceneChangeArgs
{
	int chosenSongIndex;
	int chosenDifficultyIndex; 
	
	// Score 
	int perfectNotes;
	int greatNotes;
	int missNotes; 
	int combo; 
	int maxCombo; 
	eGrade grade;
};