#ifndef BOSSPANDEMIC_H

#define BOSSPANDEMIC_H

#include "BossData.h"

#define TOT_LAYER_BACKGROUND 7 

enum SpritesPandemic
{
	MIST_SPRITE,
	SKULL_SPRITE,
	SPAWN_BOSS_SPRITE,
	BOSS_SPRITE,
	SLASH,
	PIC_SPRITE,
	RAT_SHOOT,
	RAT_BALL_SPRITE,
	SPEAR,
	BIG_RAT_SPRITE,
	CRIS,

	TOT_ANIM_PENDEMIC
};

enum StateBoss
{
	OFF,
	SPAWN_BOSS,
	START_FIGHT,
	DEAD,
};

enum BossPatern
{
	NO_PATERN,
	CHARGE,
	PIC_PATERN,
	SHOOT_PATERN,
	RAT_BALL,

	TOT_PATERN
};

enum AimPlayer
{
	START_AIM,
	LEFT_AIM = -1,
	RIGHT_AIM = 1,
	HIT_GROUND,
};

enum BossAnims
{
	IDLE_ANIM,
	PIC_ANIM,
	PIC_ANIM_SECOND_LINE,
	JUMP_START_ANIM,
	JUMP_END_ANIM,
	RAT_SHOOT_ANIM,
	TP_ANIM,
	TP_ANIM_SECOND_LINE,
	DEAD_ANIM,
	DEAD_BOUCLE,

	TOT_BOSS_ANIM,
};

struct RatShoot
{
	b2Body* body;
	float dir;
	Anim anim[1];
};

class BossPandemicData : public BossData
{
public:
	BossPandemicData();

	void Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData);
	void Display(sf::RenderWindow& _window);
	void DisplayFront(sf::RenderWindow& _window);
	void* GetDaughter() { return this; }

	void SpawnBoss(b2World* _world);
	void TurnToPlayer();

	//charge patern
	void ChargePatern(b2World* _world, float _dt);
	void HitGround(b2World* _world, CamData* _camData);
	void DestroySlash(int _slashDestroy);


	//Pic patern
	void PicPatern(b2World* _world, float _dt);
	void CratePic(b2World* _world);

	//Shoot
	void ShootPatern(b2World* _world, float _dt);

	//Rat lance
	void RatBallPatern(b2World* _world, float _dt);
	void SpearHit(b2World* _world);
	void CreateSpear(b2World* _world);
	void CreateRatBall(b2World* _world);

	//

	std::vector<int> GetVecSoundsUsed() { return m_soundsUse; }
	void ClearSoundsUsed() { m_soundsUse.clear(); }

	void Clear(b2World* _world);

	void ResetBossStatus()
	{
		m_paterns = NO_PATERN;
		m_globalTimer = 3;
		m_animState = IDLE_ANIM;
		m_offSetAnim = { 0,0 };
	}

private:

	//states
	StateBoss m_stateBoss;
	int m_paterns;

	//Charge patern
	int m_aimPlayer;
	int m_statusAnimJump;
	b2Body* m_slashs[2];

	//Pic patern
	std::vector<b2Body*> m_bodyPic;
	float m_timerPic;
	int m_picStatus;

	//Shoot patern
	bool m_stateShootPatern;

	std::vector<RatShoot> m_ratShoot;
	int m_nbRatShoot;
	float m_timerShoot;

	//Rat ball

	b2Body* m_spearBody;
	b2Body* m_ratBallBody;

	int m_statusRatBall;
	int m_dirRatBall;

	//anims
	std::map < std::string, sf::Vector2i> m_spriteSize;
	std::map < std::string, sf::Vector2f> m_spritePos;

	sf::Sprite m_backgroundLayer[TOT_LAYER_BACKGROUND];
	sf::Sprite m_spritesPandemic[TOT_ANIM_PENDEMIC];

	Anim m_mistAnim[1];
	Anim m_mistAnimBack[1];

	Anim m_skullAnim[2];
	int m_statusAnimSkull;

	Anim m_spawnBossAnim[1];

	Anim m_slashAnim[1];

	Anim m_picAnim[1];

	Anim m_ratBallAnim[1];

	Anim m_bigRatAnim[1];

	Anim m_bossAnim[TOT_BOSS_ANIM];
	int m_sizeMaxAnimBoss;
	int m_animState;

	sf::Vector2f m_turnSpriteToPlayer;
	sf::Vector2f m_offSetAnim;

	bool m_playerEated;
	std::vector<int> m_soundsUse;

	float m_timerScream;
	int m_showAttacks;


};


#endif