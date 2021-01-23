#pragma once

#include "GameObject.h"

class GameplayScene;

enum eNoteType
{
	simple,
	longNote
};

class BaseNote : public GameObject
{
public:
	BaseNote();
	virtual ~BaseNote();


	// BaseNote
	static void InitSceneRef(GameplayScene* scene);

	void BeginDrop(int x, int column);
	virtual void NoteHit() = 0;
	void EndNode();

	bool IsActive() const { return m_active; }
	int Column() const { return m_currentColumn; }
	float ElapsedFallTime() const { return m_elapsedFallTime; }
	eNoteType GetNoteType() const { return m_noteType; }

protected:
	static GameplayScene* gameplayScene;

	virtual void Activate() = 0;

	eNoteType m_noteType;

	// Sprite
	sf::Sprite * m_sprite;

	// If active
	bool m_active;

	// Falling vars
	float m_elapsedFallTime;
	float m_targetFallTime;
	float m_startY;
	float m_targetY;

	// Note size
	int m_width;
	int m_height;

	// Column
	int m_currentColumn;
};