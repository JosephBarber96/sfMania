#pragma once

#include "Note.h"

enum eLongNoteMode
{
	expanding,		// 0
	still,			// 1
	falling,		// 2
	shrinking,		// 3
};

class LongNote : public BaseNote
{
public:
	LongNote();
	~LongNote();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow * window);
	void OnSetPosition();

	// BaseNote
	void NoteHit();

	// LongNote
	void EndTail();

protected:

	// BaseNote
	void Activate();

	// LongNote
	void SetLongnoteMode(eLongNoteMode newMode);

	sf::RectangleShape* m_longBar;
	eLongNoteMode m_longNoteMode;
	float m_height;
	float m_elapsedExpandTime;
	float m_endExpandHeight;
	float m_elapsedShrinkTime;
	float m_targetShrinkTime;
	bool m_readyToEnd;

	bool m_noteSuccessfullyHeld;
	bool m_noteRegisteredMiss;
};