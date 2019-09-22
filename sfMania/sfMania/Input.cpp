#include "Input.h"

Input::Key Input::Up;
Input::Key Input::Right;
Input::Key Input::Down;
Input::Key Input::Left;

Input::Key Input::Esc;
Input::Key Input::Enter;

Input::Key Input::W;
Input::Key Input::E;
Input::Key Input::I;
Input::Key Input::O;

std::array<Input::Key*, 10> Input::Keys;

Input::Input()
{
}

Input::~Input()
{
}


void Input::Init()
{
	Up = Key();
	Right = Key();
	Down = Key();
	Left = Key();

	Esc = Key();
	Enter = Key();

	W = Key();
	E = Key();
	I = Key();
	O = Key();

	Keys = { 
		&Up, &Right, &Down, &Left, &Esc, &Enter,
		&W, &E, &I, &O 
	};
}


void Input::UpdateInput(sf::RenderWindow * window)
{
	// False out all of the key-press events
	// Remember if the key was held down the previous frame
	for (Key* key : Keys)
	{
		key->m_keyPressed = false;
		key->m_wasDown = key->m_isDown;
	}

	// Check for events (i.e - the frame a key is pressed)
	sf::Event sfevent;
	while (window->pollEvent(sfevent))
	{
		// Close window
		if (sfevent.type == sf::Event::Closed)
			window->close();

		// Key press events (The frame in which a key is pressed)
		if (sfevent.type == sf::Event::KeyPressed)
		{
			switch (sfevent.key.code)
			{
				// Nav keys
			case sf::Keyboard::Up:
				Up.m_keyPressed = true;
				break;
			case sf::Keyboard::Right:
				Right.m_keyPressed = true;
				break;
			case sf::Keyboard::Down:
				Down.m_keyPressed = true;
				break;
			case sf::Keyboard::Left:
				Left.m_keyPressed = true;
				break;

			case sf::Keyboard::Escape:
				Esc.m_keyPressed = true;
				break;
			case sf::Keyboard::Enter:
				Enter.m_keyPressed = true;
				break;

				// Gameplay keys
			case sf::Keyboard::W:
				W.m_keyPressed = true;
				break;
			case sf::Keyboard::E:
				E.m_keyPressed = true;
				break;
			case sf::Keyboard::I:
				I.m_keyPressed = true;
				break;
			case sf::Keyboard::O:
				O.m_keyPressed = true;
				break;
			}
		}
	}

	// Check if keys are currently held down
	Up.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
	Right.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
	Down.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
	Left.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Left));

	Esc.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
	Enter.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter));

	W.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::W));
	E.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::E));
	I.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::I));
	O.m_isDown = (sf::Keyboard::isKeyPressed(sf::Keyboard::O));


	// Released events
	for (Key* key : Keys)
	{
		key->m_keyReleased = (key->m_wasDown && !key->m_isDown);
	}
}