#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "Collide.h"

class PauseData
{
private:
	enum PauseMenuState
	{
		MAIN,
		SOUND,
	};

	enum ButtonName
	{
		B_RESUME,
		B_QUIT,
		B_SETTINGS,

		TOTAL_BUTTON
	};

	enum SoundElement
	{
		E_SOUND_CIRCLE,
		E_SOUND_TEXT_INFO,
		E_SOUND_BOX,
		E_SOUND_SLIDER,
		E_SOUND_CURSOR,

		TOTAL_SOUND_ELEMENT
	};

	sf::Sprite m_background;
	sf::Sprite m_spriteButton[TOTAL_BUTTON];
	sf::Sprite m_spriteElement[TOTAL_SOUND_ELEMENT];
	PauseMenuState m_state = MAIN;

	sf::Shader m_shaderClickableHover;
	sf::Clock m_clockShader;

public:
	PauseData();
	~PauseData();

	void Event(sf::Event& _event, sf::RenderWindow& _window, GameData& _gameData);
	void Update(float _dt, sf::RenderWindow& _window, GameData& _gameData);
	void Draw(sf::RenderWindow& _window, GameData& _gameData);
};

#endif