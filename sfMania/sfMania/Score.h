#pragma once

enum eGrade
{
	AAA,
	AA,
	A,
	B,
	C,
	D,
	E,
	F
};

enum eHit
{
	perfect,
	great,
	miss
};

class Score
{
public:
	Score();
	~Score();

	void AddPerfect();
	void AddGreat();
	void AddMiss();
	void SetFailed();
	eGrade CalculateGrade() const;

	// Getters
	int GetCombo() const { return m_combo; }
	int GetMaxCombo() const { return m_maxCombo; }
	int GetPerfectNotes() const { return m_perfectNotes; }
	int GetGreatNotes() const { return m_greatNotes; }
	int GetMissNotes() const { return m_missNotes; }

private:
	int m_perfectNotes;
	int m_greatNotes;
	int m_missNotes;

	int m_combo;
	int m_maxCombo;

	bool m_failed;
};