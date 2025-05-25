#include "Menu.h"

MenuData::MenuData(SoundData& _sound)
{
	_sound.StartBackgroundMusic(MAIN_MENU);

	//Load element
	m_spriteElement[M_TITLE] = LoadSprite("Sprites/Menu/Title.png", true);
	m_spriteElement[M_CIRCLE] = LoadSprite("Sprites/Menu/MenuSelectionBackground.png", true);
	m_spriteElement[M_ARROW] = LoadSprite("Sprites/Menu/Arrow.png", true);
	m_spriteElement[M_TEXTBOX] = LoadSprite("Sprites/Menu/TextBox.png", true);
	m_spriteElement[M_SELECT] = LoadSprite("Sprites/Menu/Select.png", true);

	m_spriteSound[M_SOUND_BOX] = LoadSprite("Sprites/Menu/SoundBackground.png", true);
	m_spriteSound[M_SOUND_SLIDER] = LoadSprite("Sprites/Menu/SoundRectangle.png", true);
	m_spriteSound[M_SOUND_CURSOR] = LoadSprite("Sprites/Menu/SoundCursor.png", true);

	m_spriteCredit[M_CREDIT] = LoadSprite("Sprites/Menu/Credit.png", true);
	m_spriteCredit[B_BACK] = LoadSprite("Sprites/Menu/Button/Back.png", true);


	if (sf::Joystick::isConnected(0))
	{
		m_spriteElement[M_INTERACTICON] = LoadSprite("Sprites/Input/ButtonA.png", true);
	}
	else
	{
		m_spriteElement[M_INTERACTICON] = LoadSprite("Sprites/Input/MouseLeft.png", true);
	}


	m_spriteElement[M_TITLE].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6 });
	m_spriteElement[M_CIRCLE].setPosition({ SCREEN_WIDTH / 2, Y_ANCHOR_WHEEL });
	m_spriteElement[M_ARROW].setPosition({ X_ANCHOR_WHEEL + IN_CRCL_MIDDLE_DIST * cosf(DegToRad(90.f)), Y_ANCHOR_WHEEL - IN_CRCL_MIDDLE_DIST * sinf(DegToRad(90.f)) });
	m_spriteElement[M_TEXTBOX].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10 * 9 });
	m_spriteElement[M_SELECT].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });
	m_spriteElement[M_INTERACTICON].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });

	m_spriteSound[M_SOUND_BOX].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });
	m_spriteSound[M_SOUND_SLIDER].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });
	m_spriteSound[M_SOUND_CURSOR].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });

	m_spriteCredit[M_CREDIT].setPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });
	m_spriteCredit[B_BACK].setPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });

	//Load button
	m_spriteButton[B_PLAY] = LoadSprite("Sprites/Menu/Button/Play.png", true);
	m_spriteButton[B_TUTORIAL] = LoadSprite("Sprites/Menu/Button/Tutorial.png", true);
	m_spriteButton[B_CREDIT] = LoadSprite("Sprites/Menu/Button/Credit.png", true);
	m_spriteButton[B_OPTION] = LoadSprite("Sprites/Menu/Button/Option.png", true);
	m_spriteButton[B_QUIT] = LoadSprite("Sprites/Menu/Button/Quit.png", true);
	m_spriteButton[B_SOUND] = LoadSprite("Sprites/Menu/Button/Sound.png", true);

	m_spriteButton[B_PLAY].setPosition({ X_ANCHOR_WHEEL + OUT_CRCL_MIDDLE_DIST * cosf(DegToRad(90.f)), Y_ANCHOR_WHEEL - OUT_CRCL_MIDDLE_DIST * sinf(DegToRad(90.f)) });
	m_spriteButton[B_OPTION].setPosition({ X_ANCHOR_WHEEL + OUT_CRCL_MIDDLE_DIST * cosf(DegToRad(210.f)), Y_ANCHOR_WHEEL - OUT_CRCL_MIDDLE_DIST * sinf(DegToRad(210.f)) });
	m_spriteButton[B_QUIT].setPosition({ X_ANCHOR_WHEEL + OUT_CRCL_MIDDLE_DIST * cosf(DegToRad(330.f)), Y_ANCHOR_WHEEL - OUT_CRCL_MIDDLE_DIST * sinf(DegToRad(330.f)) });

	m_spriteButton[B_SOUND].setPosition({ X_ANCHOR_WHEEL + OUT_CRCL_MIDDLE_DIST * cosf(DegToRad(90.f)), Y_ANCHOR_WHEEL - OUT_CRCL_MIDDLE_DIST * sinf(DegToRad(90.f)) });
	m_spriteButton[B_TUTORIAL].setPosition({ X_ANCHOR_WHEEL + OUT_CRCL_MIDDLE_DIST * cosf(DegToRad(210.f)), Y_ANCHOR_WHEEL - OUT_CRCL_MIDDLE_DIST * sinf(DegToRad(210.f)) });
	m_spriteButton[B_CREDIT].setPosition({ X_ANCHOR_WHEEL + OUT_CRCL_MIDDLE_DIST * cosf(DegToRad(330.f)), Y_ANCHOR_WHEEL - OUT_CRCL_MIDDLE_DIST * sinf(DegToRad(330.f)) });

	m_shaderClickableHover.loadFromFile("Shaders/Menu/ClickableHover.frag", sf::Shader::Fragment);
	m_clockShader.restart();


	m_font.loadFromFile("Font/Chapbook-Regular.otf");
	m_text.setFont(m_font);
	m_text.setCharacterSize(45);
	m_text.setString("");
	m_text.setPosition(m_spriteElement[M_TEXTBOX].getPosition());
	m_text.setFillColor(sf::Color::Black);
	m_text.setOutlineColor(sf::Color::White);
	m_text.setOutlineThickness(2);

	m_background = LoadSprite("Sprites/Menu/Background.png", false);
}

MenuData::~MenuData()
{

}

void MenuData::Event(sf::Event& _event, sf::RenderWindow& _window, GameData& _gameData)
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
	switch (m_state)
	{
	case MAIN:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == _event.KeyPressed)
		{
			_window.close();
		}
		if (m_spriteButton[B_PLAY].getGlobalBounds().contains(mousePos))
		{
			m_text.setString("Play");
			SetTextNewPos();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == _event.MouseButtonPressed)
			{
				_gameData.gameState = GAME;
				_gameData.fadingData->StartDefading(1.f);
				_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
			}
		}
		else if (m_spriteButton[B_OPTION].getGlobalBounds().contains(mousePos))
		{
			m_text.setString("Options");
			SetTextNewPos();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == _event.MouseButtonPressed)
			{
				m_state = OPTION;
				_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
			}
		}
		else if (m_spriteButton[B_QUIT].getGlobalBounds().contains(mousePos))
		{
			m_text.setString("Close the game");
			SetTextNewPos();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == _event.MouseButtonPressed)
			{
				_window.close();
				_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
			}
		}
		else
		{
			m_text.setString("");
		}
		break;
	case OPTION:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == _event.KeyPressed)
		{
			m_state = MAIN;
		}
		if (m_spriteButton[B_SOUND].getGlobalBounds().contains(mousePos))
		{
			m_text.setString("Sound");
			SetTextNewPos();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == _event.MouseButtonPressed)
			{
				m_state = SOUND;
				_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
			}
		}
		else if (m_spriteButton[B_CREDIT].getGlobalBounds().contains(mousePos))
		{
			m_text.setString("Credits");
			SetTextNewPos();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == _event.MouseButtonPressed)
			{
				m_state = CREDIT;
				_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
			}
		}
		else if (m_spriteButton[B_TUTORIAL].getGlobalBounds().contains(mousePos))
		{
			m_text.setString("Tutorial");
			SetTextNewPos();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == _event.MouseButtonPressed)
			{
				m_state = TUTORIAL;
				_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
			}
		}
		else
		{
			m_text.setString("");
		}
		break;
	case SOUND:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == _event.KeyPressed)
		{
			m_state = OPTION;
			_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
		}
		break;
	case TUTORIAL:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == _event.KeyPressed)
		{
			m_state = OPTION;
			_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
		}
		break;
	case CREDIT:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == _event.KeyPressed)
		{
			m_state = OPTION;
			_gameData.soundData->UseSound(SOURCE_MENU, BUTTON_CLICK);
		}
		break;
	default:
		break;
	}
}

void MenuData::Update(float _dt, sf::RenderWindow& _window, GameData& _gameData)
{
	m_shaderClickableHover.setUniform("timer", m_clockShader.getElapsedTime().asSeconds());

	sf::Vector2f cursorPos;
	sf::FloatRect sliderBound;
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
	float angle = atan2f(mousePos.y - (Y_ANCHOR_WHEEL), mousePos.x - (X_ANCHOR_WHEEL));
	m_spriteElement[M_ARROW].setPosition({ X_ANCHOR_WHEEL + IN_CRCL_MIDDLE_DIST * cosf(angle), Y_ANCHOR_WHEEL + IN_CRCL_MIDDLE_DIST * sinf(angle) });
	m_spriteElement[M_ARROW].setRotation(RadToDeg(angle));
	m_spriteElement[M_SELECT].setRotation(RadToDeg(angle));

	switch (m_state)
	{
	case MAIN:

		break;
	case OPTION:

		break;
	case SOUND:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			cursorPos = m_spriteSound[M_SOUND_CURSOR].getPosition();
			sliderBound = m_spriteSound[M_SOUND_SLIDER].getGlobalBounds();

			//Move the cursor pos
			if (m_spriteSound[M_SOUND_BOX].getGlobalBounds().contains(mousePos))
			{
				m_spriteSound[M_SOUND_CURSOR].setPosition({ mousePos.x, cursorPos.y });
			}
			//Update cursor pos
			cursorPos = m_spriteSound[M_SOUND_CURSOR].getPosition();

			//Check the cursor pos to his slider
			if (sliderBound.left > cursorPos.x)
			{
				m_spriteSound[M_SOUND_CURSOR].setPosition({ sliderBound.left, cursorPos.y });
			}
			if (sliderBound.left + sliderBound.width < cursorPos.x)
			{
				m_spriteSound[M_SOUND_CURSOR].setPosition({ sliderBound.left + sliderBound.width, cursorPos.y });
			}
			//Update cursor pos
			cursorPos = m_spriteSound[M_SOUND_CURSOR].getPosition();

			//std::cout << "cursor pos: " << cursorPos.x << "\tslider bound: " << sliderBound.left + sliderBound.width << "\tcursor / slider: " << (cursorPos.x - sliderBound.left) / sliderBound.width * 100 << std::endl;

			_gameData.soundData->SetVolume((cursorPos.x - sliderBound.left) / sliderBound.width * 100);
		}
		break;
	case TUTORIAL:

		break;
	case CREDIT:

		break;
	default:
		break;
	}
}

void MenuData::Draw(sf::RenderWindow& _window)
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);

	_window.draw(m_background);

	switch (m_state)
	{
	case MAIN:
		DrawElement(_window);
		_window.draw(m_text);

		for (int i = B_PLAY; i <= B_QUIT; i++)
		{
			if (m_spriteButton[i].getGlobalBounds().contains(mousePos))
			{
				_window.draw(m_spriteButton[i], &m_shaderClickableHover);
			}
			else
			{
				_window.draw(m_spriteButton[i]);
			}
		}

		break;
	case OPTION:
		DrawElement(_window);
		_window.draw(m_text);

		for (int i = B_SOUND; i <= B_CREDIT; i++)
		{
			if (m_spriteButton[i].getGlobalBounds().contains(mousePos))
			{
				_window.draw(m_spriteButton[i], &m_shaderClickableHover);
			}
			else
			{
				_window.draw(m_spriteButton[i]);
			}
		}
		break;
	case SOUND:
		//_window.draw(m_spriteElement[M_TITLE]);
		_window.draw(m_spriteElement[M_CIRCLE]);

		for (int i = 0; i < TOTAL_SOUND_ELEMENT; i++)
		{
			_window.draw(m_spriteSound[i]);
		}

		break;
	case TUTORIAL:
		//_window.draw(m_spriteElement[M_TITLE]);
		_window.draw(m_spriteElement[M_CIRCLE]);

		break;
	case CREDIT:
		//_window.draw(m_spriteElement[M_TITLE]);
		_window.draw(m_spriteElement[M_CIRCLE]);

		_window.draw(m_spriteCredit[M_CREDIT]);

		//for (int i = 0; i < TOTAL_CREDIT_ELEMENT; i++)
		//{
		//	_window.draw(m_spriteCredit[i]);
		//}
		break;
	default:
		break;
	}
}

void MenuData::SetTextNewPos()
{
	sf::FloatRect textHitbox = m_text.getGlobalBounds();
	sf::Vector2f origin = { textHitbox.width / 2, textHitbox.height / 2 };
	m_text.setOrigin(origin);
	m_text.setPosition(m_spriteElement[M_TEXTBOX].getPosition() + sf::Vector2f{ 0, -10 });
}

void MenuData::DrawElement(sf::RenderWindow& _window)
{
	for (int i = 1; i < TOTAL_ELEMENT_NAME; i++)
	{
		_window.draw(m_spriteElement[i]);
	}
}