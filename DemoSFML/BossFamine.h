#ifndef BOSSFAMINE_H
#define BOSSFAMINE_H

#include "BossData.h"

#define TABLE_SEEDFIXTURE_SIZE 4
#define TABLE_SEED_SIZE 8

#define TABLE_SEEDGROWINGFIXTURE_SIZE 11
#define TABLE_SEEDGROWING_SIZE 15

#define BOSS_SIZE_X 2
#define BOSS_SIZE_Y 2.45
#define DAMAGE_FAMINE 0


struct SeedGrow
{
	b2Body* bodyGrow;
	float timer;
};

struct Explosion
{
	sf::Vector2f spritePos;
	float timer;
};

enum Sprites
{
	BOSS_SPRITE_FAMINE,

	BACKGROUND,
	FOREGROUND,

	PROJECTILE,
	TOT_SPRITE
};

enum DummyAnim
{
	SUCTION_DUMMY,
	GROUND_DUMMY,
	STRAIGHT_DUMMY,
	IDLE_DUMMY,

	TOT_DUMMY_ANIM

};

class BossFamine : public BossData
{
public:
	BossFamine(b2World* _world, sf::Vector2f _worldPos);
	~BossFamine();

	void Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData);
	void Display(sf::RenderWindow& _window);
	void DisplayFront(sf::RenderWindow& _window);
	void* GetDaughter() { return this; }

	void Grabed() { m_grabed = true; }

	// Suction
	void SuctionInit(b2World* _world);
	void SuctionPatern(b2World* _world);

	// GroundAttack
	bool GroundAttack(b2World* _world);
	void GroundAttackPatern(b2World* _world);


	//Straight arm
	void StraightPatern(b2World* _world);

	//Clear Fight
	void ClearFight(b2World* _world);

	//tuto
	void DisplayLetter(float _dt, std::string _textToDraw);

private:

	int m_tempShowPatern;

	sf::Sprite m_spriteHelice;
	sf::Sprite m_groundFront;

	//ground
	// suction patern
	b2Body* m_bodySuction;
	int m_dir;
	//ground attack
	b2Body* m_groundAttack;
	float m_dirGround;

	//ground
	b2Body* m_straightBody;
	int m_dirStraightAttack;

	//Sprites
	sf::Sprite m_paternsSprites[TOT_SPRITE];
	Anim m_animDummy[TOT_DUMMY_ANIM];
	int m_animDummyStatus;

	// for tuto
	sf::Text m_text;
	sf::Font m_font;
	std::string m_textString;

	sf::Vector2f m_textPos;


	float m_timerText;
	int m_leterToDraw;
	int m_stepTuto;
	bool m_grabed;
};

#endif
