#include "DeathState.h"

DeathData::DeathData()
{
	m_background = LoadSprite("Sprites/DeathScreen/Background.png", true);
	m_choice[0] = LoadSprite("Sprites/DeathScreen/Continue.png", true);
	m_choice[1] = LoadSprite("Sprites/DeathScreen/Exit.png", true);
	m_select = LoadSprite("Sprites/DeathScreen/Select.png", true);
	m_looseText = LoadSprite("Sprites/DeathScreen/LooseText.png", true);
	m_backText[0] = LoadSprite("Sprites/DeathScreen/BackText.png", true);
	//We retake the already loaded texture
	m_backText[1].setTexture(*m_backText[0].getTexture());
	sf::FloatRect hitbox = m_backText[1].getGlobalBounds();
	m_backText[1].setOrigin({ hitbox.width / 2,hitbox.height / 2 });


	m_background.setPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });
	m_looseText.setPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4 });

	m_backText[0].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 3 });
	m_backText[1].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 4 });

	m_choice[0].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 3 });
	m_choice[1].setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 4 });

	m_select.setPosition({ SCREEN_WIDTH / 2, -SCREEN_HEIGHT });


	m_selectShader.loadFromFile("Shaders/Death/ClickableHover.frag", sf::Shader::Fragment);
}

DeathData::~DeathData()
{

}

void DeathData::Update(float _dt, sf::RenderWindow& _window)
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
	if (m_choice[0].getGlobalBounds().contains(mousePos))
	{
		m_select.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 3 });
	}
	else if (m_choice[1].getGlobalBounds().contains(mousePos))
	{
		m_select.setPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 4 });
	}
	else
	{
		m_select.setPosition({ SCREEN_WIDTH / 2, -SCREEN_HEIGHT });
	}

	m_selectShader.setUniform("timer", m_clockShader.getElapsedTime().asSeconds());
}

void DeathData::Draw(sf::RenderWindow& _window)
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);


	_window.draw(m_background);
	_window.draw(m_looseText);
	//_window.draw(m_backText[0]);
	//_window.draw(m_backText[1]);

	//_window.draw(m_choice[0]);
	
	if (m_choice[1].getGlobalBounds().contains(mousePos))
	{
		_window.draw(m_choice[1], &m_selectShader);
	}
	else
	{
		_window.draw(m_choice[1]);
	}

	//_window.draw(m_select);
}

void DeathData::Event(sf::Event _event,sf::RenderWindow& _window, GameData& _gameData)
{
	if (_event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(_window);
		if (m_choice[0].getGlobalBounds().contains(mousePos))
		{
			_gameData.gameState = MENU;
		}
		else if (m_choice[1].getGlobalBounds().contains(mousePos))
		{
			_gameData.gameState = MENU;
		}
	}
}