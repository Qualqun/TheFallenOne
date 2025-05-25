#ifndef BOSSDATA_H
#define BOSSDATA_H

#include "Animation.hpp"
#include "Common.h"
#include "Camera.hpp"
#include "Particle.hpp"
#include "Indicator.hpp"
#include "Sound.hpp"

class IndicatorData;
enum PhasesBoss
{
	PANDEMINE_FIRST_PHASE,
	PANDEMINE_SECOND_PHASE,
	JUMP_PHASE,
	REAPWAR_FIRST_PHASE,
	REAPWAR_SECOND_PHASE,
	TUTO_PHASE,
};

enum Bosses
{
	NO_BOSS,
	FAMINE,
	FAMINE_SECOND,
	PANDEMIC,
	DEATH_BOSS,
	JUMP_LEVEL,
	WAR
};


class BossData
{
public:

	virtual void Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData) = 0;
	virtual void Display(sf::RenderWindow& _window) = 0;
	virtual void DisplayFront(sf::RenderWindow& _window) = 0;
	virtual void* GetDaughter() = 0;

	PhasesBoss GetGamePhase() { return m_gamePhase; }
	int GetPhase() { return m_phases; }
	b2Body* GetBody() { return m_body; }

	void TakeDamage(int _nbDamage) {
		m_hp -= _nbDamage;
		m_sprite.setColor({ 255 , 100 , 100 });
		m_timerDamage = 0.7f;
	}
	int GetHp() { return m_hp; }
	int GetMaxHp() { return m_maxHp; }
	float GetTimerDamage() { return m_timerDamage; }
	void SetTimerDamage(float _f) { m_timerDamage = _f; }
	sf::Sprite* GetSprite() { return &m_sprite; }
	void SetPlayerPos(b2Vec2 _playerPos) { m_playerPos = _playerPos; }
protected:

	int m_maxHp;
	int m_hp;
	float m_timerDamage;
	float m_globalTimer; //timer used to know when a boss gonna use a new pattern
	int	m_currentPatern;
	int m_phases;
	PhasesBoss m_gamePhase;
	int m_StartRow;

	b2Body* m_body;
	int nbBoxSpawned;

	b2Vec2 m_playerPos;
	sf::Sprite m_sprite;
	sf::Sprite m_idleSprite;
	sf::Sprite m_idleSpriteWO;
};

#endif