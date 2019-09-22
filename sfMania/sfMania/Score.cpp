#include "Score.h"
#include "Maths.h"

Score::Score()
{
	m_perfectNotes = 0;
	m_greatNotes = 0;
	m_missNotes = 0;
	m_combo = 0;
	m_maxCombo = 0;
}


Score::~Score()
{
}


void Score::AddPerfect()
{
	m_perfectNotes++;
	m_combo++;
}

void Score::AddGreat()
{
	m_greatNotes++;
	m_combo++;
}

void Score::AddMiss()
{
	m_missNotes++;
	if (m_combo > m_maxCombo)
		m_maxCombo = m_combo;
	m_combo = 0;
}

eGrade Score::CalculateScore() const
{
	// SCORE CALCULATION:
	//
	// DP = Sum of score of every note.
	// Perfect : 2
	// Great : 1
	// Miss : 0
	//
	// AAA = 100% DP
	// AA = >=90% && <100% DP
	// A = >=80% && <90% DP
	// B = >=70% && <80% DP
	// C = >=60% && <70% DP
	// D = <60% DP
	// E = Song failed

	// Number of every note in song, assumed to hit perfect (x2)
	int highest_possible_dp = (m_perfectNotes + m_greatNotes + m_missNotes) * 2.0f;

	// Perfect x2 + great x1 + miss x0
	int actual_dp = (m_perfectNotes * 2.0f) + m_greatNotes;

	float percent = Maths::Normalize(actual_dp, 0, highest_possible_dp);

	if (percent >= 1.0f)
		return eGrade::AAA;
	else if (percent >= 0.9f)
		return eGrade::AA;
	else if (percent >= 0.8f)
		return eGrade::A;
	else if (percent >= 0.7f)
		return eGrade::B;
	else if (percent >= 0.6f)
		return eGrade::C;
	else
		return eGrade::D;
}