#pragma once
#include <array>

#include <SFML/Graphics.hpp>

class Input
{
public:

	struct Key
	{
		// Was the key down last frame
		bool m_wasDown;
		// Is the key down this frame
		bool m_isDown;
		// Is this the frame the key was pressed
		bool m_keyPressed;
		// Is this the frame the key was released
		bool m_keyReleased;
	};

	Input();
	~Input();

	static void Init();
	static void UpdateInput(sf::RenderWindow* window);

	// Navigation
	static Key Up;
	static Key Right;
	static Key Down;
	static Key Left;

	static Key Esc;
	static Key Enter;

	// Gameplay
	static Key W;
	static Key E;
	static Key I;
	static Key O;


private:
	static std::array<Key*, 10> Keys;
};

