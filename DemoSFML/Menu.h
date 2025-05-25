#ifndef MENU_H
#define MENU_H

#include "Animation.hpp"
#include "Update.h"

#define OUT_CRCL_MIDDLE_DIST 200
#define IN_CRCL_MIDDLE_DIST 125

#define X_ANCHOR_WHEEL SCREEN_WIDTH / 2
#define Y_ANCHOR_WHEEL SCREEN_HEIGHT / 5 * 3 +32

struct GameData;
class SoundData;

class MenuData
{
private:

	enum MenuState
	{
		MAIN,
		OPTION,
		SOUND,
		TUTORIAL,
		CREDIT,
	};

	enum ButtonName //The enum order is important for the loop in the draw
	{
		B_PLAY,
		B_OPTION,
		B_QUIT,

		B_SOUND,
		B_TUTORIAL,
		B_CREDIT,

		TOTAL_BUTTON_NAME
	};

	enum ElementName //The enum order is the order of draw in the menu
	{
		M_TITLE,
		M_CIRCLE,
		M_SELECT,
		M_ARROW,
		M_TEXTBOX,
		M_INTERACTICON,

		TOTAL_ELEMENT_NAME
	};

	enum SoundSubMenu
	{
		M_SOUND_BOX,
		M_SOUND_SLIDER,
		M_SOUND_CURSOR,

		TOTAL_SOUND_ELEMENT
	};

	enum CreditSubMenu
	{
		M_CREDIT,
		B_BACK,

		TOTAL_CREDIT_ELEMENT
	};

	sf::Sprite m_spriteButton[TOTAL_BUTTON_NAME];
	sf::Sprite m_spriteElement[TOTAL_ELEMENT_NAME];
	sf::Sprite m_spriteSound[TOTAL_SOUND_ELEMENT];
	sf::Sprite m_spriteCredit[TOTAL_CREDIT_ELEMENT];

	sf::Sprite m_background;

	sf::Font m_font;
	sf::Text m_text;

	MenuState m_state = MAIN;

	sf::Shader m_shaderClickableHover;
	sf::Clock m_clockShader;

public:
	MenuData(SoundData& _sound);
	~MenuData();

	void Event(sf::Event& _event, sf::RenderWindow& _window, GameData& _gameData);
	void Update(float _dt, sf::RenderWindow& _window, GameData& _gameData);
	void Draw(sf::RenderWindow& _window);

	void SetTextNewPos();
	void DrawElement(sf::RenderWindow& _window);
};

#endif