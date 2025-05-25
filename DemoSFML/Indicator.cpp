#include "Indicator.hpp"

#pragma region Indicator
Indicator::Indicator(sf::Vector2f _pos = { 0,0 }, IndicatorType _type = (IndicatorType)0)
{
	m_lifespan = LIFESPAN_INDICATOR;
	m_lifeTime = 0;
	m_pos = _pos;
	m_angle = 0;
	m_type = _type;
}

Indicator::~Indicator()
{
	m_pos = { NULL,NULL };
	m_lifespan = NULL;
	m_lifeTime = NULL;
	m_angle = NULL;
}
void Indicator::Update(float _dt)
{
	m_lifespan -= _dt;
}
void Indicator::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setPosition(m_pos);
	_sprite.setRotation(m_angle);

	_window.draw(_sprite);
}
#pragma endregion


#pragma region Indicator manager
IndicatorData::IndicatorData()
{
	m_sprite[0] = LoadSprite("Sprites/Indicator/Death/SpritesheetAme.png", false);
	m_sprite[1] = LoadSprite("Sprites/Indicator/Death/SpritesheetChuteComete.png", false);
	m_sprite[2] = LoadSprite("Sprites/Indicator/Death/SpritesheetExplosionComete.png", false);
	m_sprite[3] = LoadSprite("Sprites/Indicator/Death/SpritesheetFlamme.png", false);
	m_sprite[4] = LoadSprite("Sprites/Indicator/Death/SpritesheetLaser.png", false);
	m_anim[AME] = InitAnimation(sf::Vector2i(203, 202), 20, 40, 0, true);
	m_anim[CHUTE_COMETE] = InitAnimation(sf::Vector2i(198, 198), 20, 40, 0, true);
	m_anim[EXPLOSION_COMETE] = InitAnimation(sf::Vector2i(198, 198), 20, 40, 0, true);
	m_anim[FLAMME] = InitAnimation(sf::Vector2i(203, 202), 20, 40, 0, true);
	m_anim[LASER] = InitAnimation(sf::Vector2i(203, 202), 20, 40, 0, true);
}
void IndicatorData::CreateIndicator(IndicatorType _type, sf::Vector2f _pos)
{
	Indicator indicator(_pos, _type);
	m_indicatorTab.push_back(indicator);
	m_finishedPlaying = false;
}
IndicatorData::~IndicatorData()
{
	m_indicatorTab.clear();
}


void IndicatorData::Update(float _dt, BossData& _bossData)
{

	for (int i = 0; i < m_indicatorTab.size(); i++)
	{
		m_indicatorTab[i].Update(_dt);
		m_indicatorTab[i].SetPos({ _bossData.GetBody()->GetPosition().x * 64.f ,-_bossData.GetBody()->GetPosition().y * 64.f });
		Animation(m_sprite[m_indicatorTab[i].GetType()], m_anim, AME, _dt);
		if (m_indicatorTab[i].GetLifespan() <= 0)
		{
			m_indicatorTab[i].~Indicator();
			m_indicatorTab.clear();
			m_anim->timeAnim = 0;
			m_finishedPlaying = true;
		}
	}
}
void IndicatorData::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < m_indicatorTab.size(); i++)
	{
		m_indicatorTab[i].Draw(_window, m_sprite[m_indicatorTab[i].GetType()]);
	}
}
#pragma endregion