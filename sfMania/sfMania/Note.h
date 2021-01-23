#pragma once

#include "BaseNote.h"

class Note : public BaseNote
{
public:
	Note();
	~Note();

	// GameObject
	void Update();
	void RenderSelf(sf::RenderWindow * window);
	void OnSetPosition();

	// BaseNote
	void NoteHit();

protected:
	virtual void Activate();
};