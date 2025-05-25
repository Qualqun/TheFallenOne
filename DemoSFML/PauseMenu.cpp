#include "PauseMenu.hpp"

PauseData::PauseData()
{
	//Load sprite
	m_background = LoadSprite("Sprites/Pause/Background.png", true);
	m_spriteButton[B_RESUME] = LoadSprite("Sprites/Pause/Resume.png", true);
	m_spriteButton[B_QUIT] = LoadSprite("Sprites/Pause/Quit.png", true);
	m_spriteButton[B_SETTINGS] = LoadSprite("Sprites/Pause/Settings.png", true);

	m_spriteElement[E_SOUND_CIRCLE] = LoadSprite("Sprites/Menu/MenuSelectionBackground.png", true);
	m_spriteElement[E_SOUND_TEXT_INFO] = LoadSprite("Sprites/Pause/Sound.png", true);
	m_spriteElement[E_SOUND_BOX] = LoadSprite("Sprites/Menu/SoundBackground.png", true);
	m_spriteElement[E_SOUND_SLIDER] = LoadSprite("Sprites/Menu/SoundRectangle.png", true);
	m_spriteElement[E_SOUND_CURSOR] = LoadSprite("Sprites/Menu/SoundCursor.png", true);
	
	//Set sprite pos
	m_background.setPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });

	m_spriteButton[B_RESUME].setPosition({ 670 + 265,176 + 63 });
	m_spriteButton[B_QUIT].setPosition({ 826 + 143,629 + 77 });
	m_spriteButton[B_SETTINGS].setPosition({ 1557 + 119,771 + 47 });

	m_spriteElement[E_SOUND_CIRCLE].setPosition({ SCREEN_WIDTH / 2, Y_ANCHOR_WHEEL });
	m_spriteElement[E_SOUND_TEXT_INFO].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL - 100 });
	m_spriteElement[E_SOUND_BOX].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });
	m_spriteElement[E_SOUND_SLIDER].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });
	m_spriteElement[E_SOUND_CURSOR].setPosition({ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL });

	m_shaderClickableHover.loadFromFile("Shaders/Menu/ClickableHover.frag", sf::Shader::Fragment);
}

PauseData::~PauseData()
{

}

void PauseData::Event(sf::Event& _event, sf::RenderWindow& _window, GameData& _gameData)
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
	sf::Vector2f cameraOffset = _gameData.camData->GetPosition() - _gameData.camData->GetCamera().getSize() / 2.f;
	mousePos += cameraOffset;
	sf::FloatRect sliderBound = m_spriteElement[E_SOUND_SLIDER].getGlobalBounds();

	switch (m_state)
	{
	case MAIN:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == sf::Event::MouseButtonPressed)
		{
			if (m_spriteButton[B_RESUME].getGlobalBounds().contains(mousePos))
			{
				_gameData.gameState = GAME;
			}
			if (m_spriteButton[B_QUIT].getGlobalBounds().contains(mousePos))
			{
				_gameData.gameState = MENU;
				_gameData.camData->SetPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });
			}
			if (m_spriteButton[B_SETTINGS].getGlobalBounds().contains(mousePos))
			{
				m_state = SOUND;
				m_spriteElement[E_SOUND_CURSOR].setPosition({ sliderBound.left + sliderBound.width / 100 * _gameData.soundData->GetVolume(), sliderBound.top + sliderBound.height / 2 });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == sf::Event::KeyPressed)
		{
			_gameData.gameState = GAME;
		}
		break;
	case SOUND:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && _event.type == sf::Event::KeyPressed)
		{
			m_state = MAIN;
		}
		break;
	default:
		break;
	}
}

void PauseData::Update(float _dt, sf::RenderWindow& _window, GameData& _gameData)
{
	m_shaderClickableHover.setUniform("timer", m_clockShader.getElapsedTime().asSeconds());

	sf::Vector2f cursorPos;
	sf::FloatRect sliderBound;
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
	sf::Vector2f cameraOffset = _gameData.camData->GetPosition() - _gameData.camData->GetCamera().getSize() / 2.f;
	mousePos += cameraOffset;

	switch (m_state)
	{
	case MAIN:

		break;
	case SOUND:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			cursorPos = m_spriteElement[E_SOUND_CURSOR].getPosition();
			sliderBound = m_spriteElement[E_SOUND_SLIDER].getGlobalBounds();

			//Move the cursor pos
			if (m_spriteElement[E_SOUND_BOX].getGlobalBounds().contains(mousePos))
			{
				m_spriteElement[E_SOUND_CURSOR].setPosition({ mousePos.x, sliderBound.top + sliderBound.height / 2 });
			}
			//Update cursor pos
			cursorPos = m_spriteElement[E_SOUND_CURSOR].getPosition();

			//Check the cursor pos to his slider
			if (sliderBound.left > cursorPos.x)
			{
				m_spriteElement[E_SOUND_CURSOR].setPosition({ sliderBound.left, cursorPos.y });
			}
			if (sliderBound.left + sliderBound.width < cursorPos.x)
			{
				m_spriteElement[E_SOUND_CURSOR].setPosition({ sliderBound.left + sliderBound.width, cursorPos.y });
			}
			//Update cursor pos
			cursorPos = m_spriteElement[E_SOUND_CURSOR].getPosition();

			_gameData.soundData->SetVolume((cursorPos.x - sliderBound.left) / sliderBound.width * 100);
		}
		break;
	default:
		break;
	}

	//Move all element to follow the camera
	m_background.setPosition(sf::Vector2f{ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 } + cameraOffset);

	m_spriteButton[B_RESUME].setPosition(sf::Vector2f{ 670 + 265,176 + 63 } + cameraOffset);
	m_spriteButton[B_QUIT].setPosition(sf::Vector2f{ 826 + 143,629 + 77 } + cameraOffset);
	m_spriteButton[B_SETTINGS].setPosition(sf::Vector2f{ 1557 + 119,771 + 47 } + cameraOffset);

	m_spriteElement[E_SOUND_CIRCLE].setPosition(sf::Vector2f{ SCREEN_WIDTH / 2, Y_ANCHOR_WHEEL } + cameraOffset);
	m_spriteElement[E_SOUND_TEXT_INFO].setPosition(sf::Vector2f{ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL - 100 } + cameraOffset);
	m_spriteElement[E_SOUND_BOX].setPosition(sf::Vector2f{ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL } + cameraOffset);
	m_spriteElement[E_SOUND_SLIDER].setPosition(sf::Vector2f{ X_ANCHOR_WHEEL, Y_ANCHOR_WHEEL } + cameraOffset);
	sliderBound = m_spriteElement[E_SOUND_SLIDER].getGlobalBounds();

}

void PauseData::Draw(sf::RenderWindow& _window, GameData& _gameData)
{
	_window.draw(m_background);

	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
	sf::Vector2f cameraOffset = _gameData.camData->GetPosition() - _gameData.camData->GetCamera().getSize() / 2.f;
	mousePos += cameraOffset;

	switch (m_state)
	{
	case MAIN:

		for (int i = 0; i < TOTAL_BUTTON; i++)
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

		for (int i = 0; i < TOTAL_SOUND_ELEMENT; i++)
		{
			_window.draw(m_spriteElement[i]);
		}

		break;
	default:
		break;
	}
}