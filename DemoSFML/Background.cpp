#include "Background.hpp"

BackgroundData::BackgroundData()
{
	//LoadPandemicBackground();
	//LoadDeathBackground();

}

BackgroundData::~BackgroundData()
{	

}

void BackgroundData::Update(float _dt, sf::Vector2f _posCam)
{
	m_timer += _dt;
	if (m_phaseBackground == DEATHBOSS)
	{
		m_shaderBackground[1]->setUniform("compteur", m_timer);
		m_shaderMiddleground[1]->setUniform("time", m_timer);
	}

	AnimBackground(_dt);
}

void BackgroundData::DrawBackground(sf::RenderWindow& _window, sf::Vector2f _posCam)
{
	for (unsigned int i = 0; i < m_background.size(); i++)
	{
		if (m_shaderBackground.size() > i)
		{
			if (m_shaderBackground[i] == nullptr)
			{
				_window.draw(m_background[i]);
			}
			else
			{
				_window.draw(m_background[i], &*m_shaderBackground[i]);
			}
		}
		else
		{
			_window.draw(m_background[i]);
		}
	}
}

void BackgroundData::AnimMist(float _dt)
{
	float frameTime;
	float frameNb;

	frameTime = 1.f / m_mistAnim[m_phaseBackground].framerate;
	frameNb = m_mistAnim->timeAnim / frameTime;

	if (m_mistAnim->timeAnim >= 0.75f && m_currentMistAnim <= 3)
	{
		if (m_currentMistAnim <= 3)
		{
			m_mistAnim->timeAnim = 0;
			m_currentMistAnim++;
		}
		else
		{
			m_mistAnim->timeAnim = 0;
			m_currentMistAnim = 0;
		}
	}

	else if (m_mistAnim->timeAnim >= 0.55f && m_currentMistAnim == 4)
	{
		if (m_currentMistAnim <= 3)
		{
			m_mistAnim->timeAnim = 0;
			m_currentMistAnim++;
		}
		else
		{
			m_mistAnim->timeAnim = 0;
			m_currentMistAnim = 0;
		}
	}

	// MIST 2
	frameTime = 1.f / m_mistAnim2[m_phaseBackground].framerate;
	frameNb = m_mistAnim2->timeAnim / frameTime;

	if (m_mistAnim2->timeAnim >= 0.75f && m_currentMistAnim2 <= 3)
	{
		if (m_currentMistAnim2 <= 3)
		{
			m_mistAnim2->timeAnim = 0;
			m_currentMistAnim2++;
		}
		else
		{
			m_mistAnim2->timeAnim = 0;
			m_currentMistAnim2 = 0;
		}
	}

	else if (m_mistAnim2->timeAnim >= 0.55f && m_currentMistAnim2 == 4)
	{
		if (m_currentMistAnim2 <= 3)
		{
			m_mistAnim2->timeAnim = 0;
			m_currentMistAnim2++;
		}
		else
		{
			m_mistAnim2->timeAnim = 0;
			m_currentMistAnim2 = 0;
		}
	}
	AnimationChain(m_middleground[0], m_mistAnim, m_currentMistAnim, _dt);
	AnimationChain(m_middleground[1], m_mistAnim2, m_currentMistAnim2, _dt / 2);
}

void BackgroundData::AnimBackground(float _dt)
{
	float frameTime;
	float frameNb;
	switch (m_phaseBackground)
	{
	case PANDEMICBOSS:

		AnimMist(_dt);
		break;
	case FAMINEBOSS:




		break;
	case WARBOSS:




		break;
	case DEATHBOSS:



		break;
	default:
		break;
	}
}
void BackgroundData::DrawMist(sf::RenderWindow& _window, sf::Vector2f _posCam, int _nbLayer)
{
	if (_nbLayer == 0)
	{
		_window.draw(m_middleground[0]);
		m_middleground[0].setPosition({ -SCREEN_WIDTH , 545 });
		_window.draw(m_middleground[0]);
		m_middleground[0].setPosition({ -SCREEN_WIDTH * 2 , 545 });
		_window.draw(m_middleground[0]);
		m_middleground[0].setPosition({ 0 , 545 });
	}
	else if (_nbLayer == 1)
	{
		_window.draw(m_middleground[1]);
		m_middleground[1].setPosition({ -800 + SCREEN_WIDTH, 545 });
		_window.draw(m_middleground[1]);
		m_middleground[1].setPosition({ -800 - SCREEN_WIDTH, 545 });
		_window.draw(m_middleground[1]);
		m_middleground[1].setPosition({ -800 - SCREEN_WIDTH * 2, 545 });
		_window.draw(m_middleground[1]);
		m_middleground[1].setPosition({ -800 - SCREEN_WIDTH * 3, 545 });
		_window.draw(m_middleground[1]);
		m_middleground[1].setPosition({ -800 , 545 });
	}
}

void BackgroundData::DrawMiddleground(sf::RenderWindow& _window, sf::Vector2f _posCam)
{
	for (unsigned int i = 0; i < m_middleground.size(); i++)
	{
		if (m_shaderMiddleground.size() > i)
		{
			if (m_shaderMiddleground[i] == nullptr)
			{
				_window.draw(m_middleground[i]);
			}
			else
			{
				_window.draw(m_middleground[i], &*m_shaderMiddleground[i]);
			}
		}
		else
		{
			_window.draw(m_middleground[i]);
		}

		if (m_phaseBackground == PANDEMICBOSS)
		{
			if (i == 0)
			{
				DrawMist(_window, _posCam, i);

			}
			else if (i == 1)
			{
				DrawMist(_window, _posCam, i);
			}
		}
	}
}

void BackgroundData::DrawForeground(sf::RenderWindow& _window, sf::Vector2f _posCam)
{
	for (unsigned int i = 0; i < m_foreground.size(); i++)
	{
		if (m_shaderForeground.size() > i)
		{
			if (m_shaderForeground[i] == nullptr)
			{
				_window.draw(m_foreground[i]);
			}
			else
			{
				_window.draw(m_foreground[i], &*m_shaderForeground[i]);
			}
		}
		else
		{
			_window.draw(m_foreground[i]);
		}
	}

}

void BackgroundData::AddBackground(sf::Sprite& _sprite, sf::Vector2f _pos)
{
	_sprite.setPosition(_pos);
	m_background.push_back(_sprite);
}

void BackgroundData::AddMiddleground(sf::Sprite& _sprite, sf::Vector2f _pos)
{
	_sprite.setPosition(_pos);
	m_middleground.push_back(_sprite);
}

void BackgroundData::AddForeground(sf::Sprite& _sprite, sf::Vector2f _pos)
{
	_sprite.setPosition(_pos);
	m_foreground.push_back(_sprite);
}

//Shader overload
void BackgroundData::AddBackground(sf::Sprite& _sprite, sf::Vector2f _pos, sf::Shader& _shader)
{
	_sprite.setPosition(_pos);
	m_background.push_back(_sprite);
	m_shaderBackground.resize(m_background.size() - 1);
	m_shaderBackground.push_back(&_shader);
}

void BackgroundData::AddMiddleground(sf::Sprite& _sprite, sf::Vector2f _pos, sf::Shader& _shader)
{
	_sprite.setPosition(_pos);
	m_middleground.push_back(_sprite);
	m_shaderMiddleground.resize(m_background.size() - 1);
	m_shaderMiddleground.push_back(&_shader);
}

void BackgroundData::AddForeground(sf::Sprite& _sprite, sf::Vector2f _pos, sf::Shader& _shader)
{
	_sprite.setPosition(_pos);
	m_foreground.push_back(_sprite);
	m_shaderForeground.resize(m_background.size() - 1);
	m_shaderForeground.push_back(&_shader);
}

void BackgroundData::CleanAllGround()
{
	m_background.clear();
	m_middleground.clear();
	m_foreground.clear();

	m_shaderBackground.clear();
	m_shaderMiddleground.clear();
	m_shaderForeground.clear();
}
void BackgroundData::LoadDeathBackground()
{
	m_phaseBackground = DEATHBOSS;
}
void BackgroundData::LoadPandemicBackground()
{
	for (int i = 0; i < 4; i++)
	{
		m_mistAnim[i] = InitAnimation(sf::Vector2i(1920, 535), 10, 8, 0, false);
	}
	for (int i = 0; i < 4; i++)
	{
		m_mistAnim2[i] = InitAnimation(sf::Vector2i(1920, 535), 10, 8, 0, false);
	}
	m_currentMistAnim = 0;
	m_mistAnim[4] = InitAnimation(sf::Vector2i(1920, 535), 10, 6, 0, true);
	m_mistAnim2[4] = InitAnimation(sf::Vector2i(1920, 535), 10, 6, 0, true);

	sf::Sprite m_mistSprite = LoadSprite("Sprites/Map/Pandemic/MistAnimeBg.png", false);
	AddMiddleground(m_mistSprite, { 0 , 545 });
	AddMiddleground(m_mistSprite, { 0 , 545 });
	m_phaseBackground = PANDEMICBOSS;
}

void BackgroundData::LoadJumpBackground()
{
	m_phaseBackground = JUMP;
	sf::Sprite tempBackSprite = LoadSprite("Bibouba", false);
	sf::Vector2f tempBackVec(7776, -1920);
	AddBackground(tempBackSprite, tempBackVec);
}