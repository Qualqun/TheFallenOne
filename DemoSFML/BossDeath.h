#ifndef BOSSDEATH_H
#define BOSSDEATH_H

#include "BossData.h"

#define BOSS_DEATH_MAX_HEALTH 500
#define BOSS_GLOBAL_TIMER 3.0f
#define PROJECTILE_SPEED 10
#define TIMER_TRANSFERT_PATERN 2.f

#define BOSS_WIDTH 250
#define BOSS_HEIGHT 500

#define BOSS_MIN_VELOCITY  10
#define BOSS_MAX_VELOCITY  190

#define BOSS_MAX_X 1792 / 64.f
#define BOSS_MIN_X 300 / 64.f

#define BOSS_DEATH_SPAWN_X 960 // 64.f
#define BOSS_DEATH_SPAWN_Y 400 // 64.f

#define SEA_SOUL_DURATION 5.f

#define FIRE_LIFESPAN 1.35f //1.8f
#define FIRE_WIDTH 261	//391
#define FIRE_HEIGHT	255	//382
#define FIRE_WIDTH_B2D 256 / ZOOM
#define FIRE_HEIGHT_B2D FIRE_HEIGHT / ZOOM.f

#define SOUL_PROJ_BASE_SPEED 20.f
#define SOUL_PROJ_MIN_SPEED 5.f
#define SOUL_PROJ_COOLDOWN 1.f

#define EXPLOSION_TIMER 3.f
#define EXPLOSION_RADIUS 20.f
#define BLAST_TIME 2.f

#define LASER_WIDTH 64.f
#define LASER_HEIGHT 512.f
#define LASER_WIDTH_B2D (LASER_WIDTH / ZOOM)
#define LASER_HEIGHT_B2D (LASER_HEIGHT / ZOOM)

#define DAMAGE_ZONE_TIMER 7.f

//Balls
#define BALLS_PATERN_TIME 5.f
#define BALLS_MAX_NB 10
#define BALLS_ROTATION_SPEED (M_PI * 2)
#define BALLS_SPEED 17.f
#define BALLS_RADIUS 1.f
#define BALLS_COOLDOWN 0.1f

//Patern Circle Shriking
#define NB_GHOST_CIRCLEPATERN 64
#define CIRCLE_SHRIKING_SPEED 10.0f
#define DISTANCE_CIRCLESHRIEKING 1200.f
#define GAP_INTERVAL 80

//Patern Comet
#define COMET_COOLDOWN 1.0f
#define COMET_SPEED 40.0f
#define BOSS_COMET_EXPLO_WIDTH 100
#define BOSS_COMET_EXPLO_HEIGHT 150
#define TIMER_COMET_EXPLO 1.f

#define SAFE_ZONE_RADIUS 3.f

#define TIME_TO_TP 0.35f

#define TOTAL_ANIM_LINE 7
enum BossDeathDir
{
	BOSS_LEFT_DIR,
	BOSS_RIGHT_DIR
};
enum BossDeathAnim
{
	FRONT_IDLE_P1,
	SIDE_IDLE_P1,
	FRONT_ATTACK_P1,
	SIDE_ATTACK_P1,

	FRONT_IDLE_P2,
	SIDE_IDLE_P2,
	FRONT_ATTACK_P2,
	SIDE_ATTACK_P2,
	TOTAL_ANIMATION_DEATH

};
class Comet
{
	b2Body* m_body;
	float m_coolDown;
};

enum CometAnimations
{
	FALLING,
	EXPLODING
};

enum StateAttackDeath
{
	//Phase 1
	SEA_SOULS,
	SOUL_PROJECTION,
	SHRINKING_CIRCLE,
	FIRE_GROUND,
	SHOCK_WAVE,

	PHASE2,

	//Phase 2
	COMET,
	BOUNCING_BALLS,
	DAMAGE_ZONE,
	LASER_THEN_DASH,
	BLAST_CENTER,

	IDLE_DEATH,

};

class Cristal
{
private:
	b2Body* m_body;
public:
	Cristal() {};
	Cristal(b2World* _world, b2Vec2 _pos, unsigned int _index);
	~Cristal();

	b2Body* GetBody() { return m_body; }
	b2Vec2 GetCristalPos() { return m_body->GetPosition(); }
};

class CristalData : public Cristal
{
private:
	std::vector<Cristal*> m_cristalList;
	std::vector<b2Body*> m_safeZone;
	sf::Sprite m_spriteCristal;
	sf::Sprite m_spriteSafezone;
public:
	CristalData();
	~CristalData();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window);

	void CreateCristal(b2World* _world, b2Vec2 _pos);
	void CreateSafezone(b2World* _world, b2Vec2 _pos);

	void CristalExplode(b2World* _world, unsigned int _index, bool _createSafezone);

	void DestroyAllElement(b2World* _world);
};

class BossDeath : public BossData
{
private:
	//Private struct and enum
	enum DeathAnimations
	{
		INTRO_ANIM,
		IDLE_ANIM,
		TOTAL_ANIM,
		IDLE_SIDE_ANIM
	};

	//Spawn
	bool m_trigerSpawn;
	float m_timerSpawn;
	float m_timeToSpawn;

	//Attack indicator
	sf::Sprite m_introBoss;

	bool m_atkInit = false;

	//Fire ground
	sf::Sprite m_fireSprite;
	std::vector<b2Body*> m_fire;
	B2Dinfo m_fireInfo;
	std::vector<float> m_fireLifetime;
	Anim m_fireAnim[1];
	std::vector<sf::IntRect> m_fireIntRect;
	std::vector<bool> m_fireBodyDestroyed;

	//Ajout récent
	b2Body* m_ExploCometBody;
	b2Body* m_cometBody;
	int m_cumulProj = 0;
	std::vector<float> m_animTimingComet;
	sf::Sprite backGround;
	std::vector<b2Body*> m_ghostTab;
	sf::Sprite m_ghostSprite;
	b2Body* m_bodyCenter;
	float m_timerCometExplo;
	float m_offSetVelocityY;
	bool m_isGoingDown;
	bool m_direction;
	bool m_isInSide;

	//Soul proj
	b2Body* m_projection;
	bool m_soulGoBack = false;
	float m_soulTimer = SOUL_PROJ_COOLDOWN;

	//Sea souls
	std::vector<b2Body*> m_seaSoul;
	std::vector<float> m_seaTimer;
	b2Vec2 m_seaOffset;
	sf::Sprite m_seaSprite;

	//Damage zone
	sf::Sprite m_damageZoneSprite;
	b2Body* m_damageZone;
	float m_damageZoneTimer;
	bool m_damageZoneArrived;

	//Bouncing balls
	float m_ballsPaternTimer;
	bool m_cristalSpawned;
	std::vector<b2Body*> m_ballsBody;
	float m_ballsCooldown;
	sf::Sprite m_ballsSprite;
	B2Dinfo m_ballsInfo;

	//Blast zone
	float m_explosionTimer;
	float m_blastTimer;
	b2Body* m_explosion;
	bool m_exploded;

	b2Body* m_laser;
	sf::Sprite m_laserSprite;
	int m_dirLaser;

	//Tp info
	b2Vec2 m_posMemory;
	b2Vec2 m_tpTarget;
	bool m_isTeleported = false;
	bool m_isTeleporting = false;
	bool m_finishedTp = true;
	float m_tpTime;
	float m_timerTp;


	//Anim Death Spells
	Anim m_cometAnim[2];
	Anim m_explocometAnim;
	Anim m_laserAnim;

	sf::Sprite m_cometSprite;
	sf::Sprite m_explocometSprite;

	//Cristal Manager
	CristalData* m_cristalData;

	//Delete the random of the attack by making pattern cycle
	unsigned int m_atk = 0;

	//Tell if the boss should float automaticly around or not
	bool m_doMove = true;

	BossDeathAnim m_currentAnim = FRONT_IDLE_P1;

	//Ajout
	sf::Sprite m_bossIntro;
	Anim m_spawnAnim[TOTAL_ANIM_LINE];
	Anim m_bossAnim[TOTAL_ANIMATION_DEATH];
	bool m_isAnimFinished;
	sf::Sprite m_tempBossSprite;

	bool m_didShake = false;

public:
	BossDeath(b2World* _world, sf::Vector2f _pos);
	~BossDeath();

	void Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData);
	void Display(sf::RenderWindow& _window);

	void DisplayFront(sf::RenderWindow& _window) {};

	void* GetDaughter() { return this; }

	void ResetAtkState();
	void ResetAtkStateTp();

	//Patern phase 1
	void SeaSoulsPatern(b2World* _world, float _dt);
	void SoulProjectionPatern(b2World* _world, float _dt);
	void ShrinkingCirclePatern();
	void FireGroundPatern(b2World* _world, float _dt);
	void ShockWavePatern();

	//Patern phase 2
	void CometPatern();
	void BouncingBallsPatern(b2World* _world, float _dt);
	void DamageZonePatern(b2World* _world, float _dt);
	void LaserThenDashPatern(b2World* _world);
	void BlastCenterPatern(b2World* _world, float _dt, CamData& _camData, ParticleData& _particleData);


	//Init phase 1
	void SeaSoulsInit(b2World* _world);
	void SoulProjectionInit(b2World* _world);
	void ShrinkingCircleInit(b2World* _world);
	void FireGroundInit(b2World* _world);
	void ShockWaveInit(b2World* _world);

	//Init phase 2
	void CometInit(b2World* _world);
	void BouncingBallsInit(b2World* _world);
	void DamageZoneInit(b2World* _world);
	void LaserThenDashInit(b2World* _world);
	void BlastCenterInit(b2World* _world);

	//Init Laser

	void LaserInit() { ; }

	void ResetCometAnim() { m_cometAnim->timeAnim = 0; }
	//CometExploInit
	void CometExploInit(b2Vec2 _pos, b2World* _world);

	void DeleteGhostTab(int _i, b2World* _world);
	void MovementBoss(b2World* _world, float _dt);
	void Teleport(b2Vec2 _destination , bool _isSided);
	void TpUpdate(float _dt);

	CristalData* GetCristalData() { return m_cristalData; }

	std::vector<b2Body*>* GetBallsVector() { return &m_ballsBody; }
	bool GetBallsShouldBounce() { return m_ballsPaternTimer > 0 ? true : false; }
};
#endif