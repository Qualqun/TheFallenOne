#ifndef INDICATOR_HPP
#define INDICATOR_HPP

#include "Common.h"
#include "Animation.hpp"
#include "BossData.h"

#define LIFESPAN_INDICATOR 2.0f

class BossData;
enum IndicatorType
{
	AME,
	CHUTE_COMETE,
	EXPLOSION_COMETE,
	FLAMME,
	LASER,
	TOTAL_INDICATOR

};
//Particle base container
class Indicator
{
private:

protected:

	float m_lifespan;
	float m_lifeTime;
	int m_type;
	Anim m_anim;
	sf::Vector2f m_pos;
	float m_angle;


public:
	Indicator(sf::Vector2f _pos, IndicatorType _type);
	~Indicator();

	void Update(float _dt);
	virtual void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite);

	float GetLifespan() { return m_lifespan; }
	float GetLifeTime() { return m_lifeTime; }
	int GetType() { return m_type; }
	sf::Vector2f GetPos() { return m_pos; }
	void SetPos(sf::Vector2f _vec) { m_pos = _vec; }
	float GetAngle() { return m_angle; }

	void AddLifeTime(float _time) { m_lifeTime += _time; }
};

//Particle manager
class IndicatorData
{
private:
	bool m_finishedPlaying = true;
	std::vector<Indicator> m_indicatorTab;
protected:

	sf::Sprite m_sprite[TOTAL_INDICATOR];
	Anim m_anim[TOTAL_INDICATOR];

public:
	IndicatorData();
	~IndicatorData();

	void Update(float _dt, BossData& _bossData);
	void Draw(sf::RenderWindow& _window);

	void CreateIndicator(IndicatorType _type, sf::Vector2f _pos);
	bool FinishedPlaying() { return m_finishedPlaying; }

	//Particle creator
};
#endif