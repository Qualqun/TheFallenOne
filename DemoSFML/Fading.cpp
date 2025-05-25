#include "Fading.hpp"

FadingData::FadingData()
{
	m_fade.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	m_fade.setFillColor(sf::Color(0, 0, 0, 255));
	m_fade.setOrigin({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });
	m_fade.setPosition(sf::Vector2f{ 0,SCREEN_HEIGHT });
}

FadingData::~FadingData()
{

}

void FadingData::Update(float _dt, CamData& _camData)
{
	if (m_isFinished == false)
	{
		m_timer -= _dt;
		if (m_timer <= 0)
		{
			m_isFinished = true;
			m_timer = 0;
		}

		float alpha = 0;
		switch (m_type)
		{
		case FADE:
			//Set the alpha of the rectangle, it start at 0 and go to 255
			alpha = 255 - (m_timer / m_timeTotal) * 255;
			break;
		case UNFADE:
			//Set the alpha of the rectangle, it start at 255 and go to 0
			alpha = (m_timer / m_timeTotal) * 255;
			break;
		default:
			break;
		}

		m_fade.setFillColor(sf::Color(0, 0, 0, alpha));
		m_fade.setPosition(_camData.GetPosition());
	}
}

void FadingData::Draw(sf::RenderWindow& _window)
{
	_window.draw(m_fade);
}

void FadingData::StartFading(float _timeToFade = 1)
{
	m_timeTotal = _timeToFade;
	m_timer = _timeToFade;
	m_isFinished = false;
	m_type = FADE;
}

void FadingData::StartDefading(float _timeToUnfade = 1)
{
	m_timeTotal = _timeToUnfade;
	m_timer = _timeToUnfade;
	m_isFinished = false;
	m_type = UNFADE;
}