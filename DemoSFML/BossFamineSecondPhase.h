#ifndef BOSSFAMINESECONDPHASE_H
#define BOSSFAMINESECONDPHASE_H

#include "BossData.h"


enum AllStatusBoss
{
	GRAB_PLATFORM_LEFT,
	GRAB_PLATFORM_RIGHT,
	RECOVER,
	BOSS_DEAD
};

struct Flufekey
{
	b2Body* body;
	b2Vec2 velocityBody;
	float timerDash;
};

enum AllAnim
{
	GROUND_ANIM = 1,
	MOUNTH_ANIM = 1,

	TENTACLE_IDLE = 0,
	TENTACLE_GRAB,
	TENTACLE_UNGRAB,

	TENTACLE_TOT_ANIM,


	EYE_TEAR_IDLE = 0,
	EYE_TEAR_OPEN,
	EYE_TEAR_CLOSE,
	EYE_TEAR_CRY,
	EYE_TEAR_DEAD,

	EYE_TOT_ANIM,

};

class BossFamineSecondPhase : public BossData
{
public:

	BossFamineSecondPhase(sf::Vector2f _pos, b2World* _world);

	void Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData);
	void Display(sf::RenderWindow& _window);
	void DisplayFront(sf::RenderWindow& _window);

	void* GetDaughter() { return this; }
	bool GetCanHit() { return m_canHit; }
	void TentacleTakeDamage();

	void TearsHit(b2World* _world, sf::Vector2f _pos);

	void CreateFlufekey(b2World* _world);
	void FlufekeyUpdate(float _dt);

	void CreateTentacle(int _platform, b2World* _world);
	void PlatformMove(int _dir, int _platformSelected);

	void CreateMount(b2World* _world);
	void ClearFight(b2World* _world);
private:

	sf::Sprite m_backGround;
	bool m_canHit;
	bool m_destroyWall;

	int m_tentacleHp;
	int m_status;
	b2Body* m_platforms[2];

	// Mounth
	b2Body* m_mounthBody[2];
	float m_timerMouth;

	//FLUFEKEY OYOOOOOOOO
	std::vector<Flufekey> m_flufekeyVec;
	float m_timerSpawnFlufekey;

	// Tears
	float m_timerTears;
	bool m_spawnTears;
	std::vector<b2Body*> m_tearsGroundVec;


	// anim
	sf::Shader m_shaderChainBalance[2];

	sf::Sprite m_eyeTearSpritesheet;
	Anim m_animEye[EYE_TOT_ANIM];
	int m_stateAnimEye;

	sf::Sprite m_spritesheetGround;
	Anim m_animGround[GROUND_ANIM];

	sf::Sprite m_spritesheetBalance[2];
	Anim m_animBalance[2][TENTACLE_TOT_ANIM];
	int m_stateAnimBalance[2];

	sf::Sprite m_tearSpritesheet;
	std::vector<Anim*> m_animTear;

	sf::Sprite m_spawnFlufekeySpritesheet;
	Anim m_animSpawnFlufekey[1];
	bool m_flufekeySpawn;

	sf::Sprite m_spritesheetFlufekey;
	std::vector<Anim*> m_animFlufekey;

	sf::Sprite m_spritesheetMounth;
	Anim m_animMounth[MOUNTH_ANIM];

	sf::Sprite m_backgroundSpritesheet;
	Anim m_animBackground[1];


	sf::Sprite m_penduleSprite;

	std::map < std::string, sf::Vector2i> m_spriteSize;
	std::map < std::string, sf::Vector2f> m_spritePos;

	//std::vector<Anim*> m_animFlufekey;


};

#endif