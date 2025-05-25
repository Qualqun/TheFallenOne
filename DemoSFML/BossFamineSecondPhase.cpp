#include "BossFamineSecondPhase.h"



#define TENTACLE_SIZE_X 1
#define TENTACLE_SIZE_Y 1.5
#define TENTACLE_HP 3

#define PLATFORM_RIGHT 0
#define PLATFORM_LEFT 1

#define PLATFORM_WIDTH 6.85
#define PLATFORM_HEIGHT -0.5

#define TENTACLES_FRAMES 20

#define PLATFORM_SPRITE_LEFT_X 910
#define PLATFORM_SPRITE_RIGHT_X 1795
#define PLATFORM_SPRITE_Y 330 


#define PLATFORM_RIGHT_X 31.7
#define PLATFORM_LEFT_X 3.7
#define PLATFORM_MAX_Y -14.6 
#define PLATFORM_MIN_Y -16.5 

#define TIMER_RECOVER 1.5+9.5

#define MOUNTH_DISPAWN 2.5
#define MOUNTH_RIGHT 0
#define MOUNTH_LEFT 1
#define MOUNTH_OFFSET_SPAWN_X -2.25
#define MOUNTH_OFFSET_SPAWN_Y 1
#define MOUNTH_SPEED 5
#define TIMER_MOUNTH 5 

#define TIMER_SPAWN_FLUFEKEY 14.5
#define FLUFEKEY_ACCELERATION 9
#define FLUFEKEY_MAX_SPEED 8

#define TIME_SPAWN_TEARS 2.25
#define TEARS_SPEED 4


enum SpawnFlufy
{
	SPAWN_TOP,
	SPAWN_DOWN,
	SPAWN_RIGHT,
	SPAWN_LEFT
};

enum Balance
{
	BALANCE_RIGHT,
	BALANCE_LEFT
};

enum DirPlatform
{
	DOWN = -1,
	UP = 1
};

BossFamineSecondPhase::BossFamineSecondPhase(sf::Vector2f _pos, b2World* _world)
{
	B2Dinfo bossInfo;
	B2Dinfo plateformInfo;
	sf::Vector2f plateformSize = { PLATFORM_WIDTH, PLATFORM_HEIGHT };

	m_hp = 100;
	m_destroyWall = true;

	// create body boss
	bossInfo.type = b2_kinematicBody;
	bossInfo.shape = CIRCLE;
	bossInfo.userData.fixtureName = "FamineSecondPhaseBoss";
	bossInfo.userData.interruptGrapin = false;
	bossInfo.pos = _pos;
	bossInfo.radius = 1.4;
	bossInfo.density = 20;
	bossInfo.friction = 1;
	bossInfo.fixedRotation = true;

	m_body = CreateBodyB2D(_world, bossInfo);
	m_body->SetGravityScale(0);
	m_body->GetFixtureList()->SetSensor(true);

	// create bodyPlateform left 
	plateformInfo.type = b2_kinematicBody;
	plateformInfo.shape = POLYGON;
	plateformInfo.userData.fixtureName = "Platform";
	plateformInfo.pos = sf::Vector2f(PLATFORM_LEFT_X, PLATFORM_MAX_Y);
	plateformInfo.points.push_back(b2Vec2(0, 0));
	plateformInfo.points.push_back(b2Vec2(plateformSize.x, 0));
	plateformInfo.points.push_back(b2Vec2(plateformSize.x, plateformSize.y));
	plateformInfo.points.push_back(b2Vec2(0, plateformSize.y));
	plateformInfo.density = 1;
	plateformInfo.friction = 1;
	plateformInfo.fixedRotation = false;

	m_platforms[PLATFORM_LEFT] = CreateBodyB2D(_world, plateformInfo);

	// create bodyPlateform right 
	plateformInfo.userData.fixtureName = "Platform";
	plateformInfo.pos = sf::Vector2f(PLATFORM_RIGHT_X, PLATFORM_MAX_Y);

	m_platforms[PLATFORM_RIGHT] = CreateBodyB2D(_world, plateformInfo);

	// end create bodyPlateform right

	m_globalTimer = TIMER_RECOVER /*TIMER_RECOVER*/;
	m_status = RECOVER;
	m_canHit = true;

	m_timerSpawnFlufekey = 0;
	m_tentacleHp = 3;


	m_gamePhase = PANDEMINE_SECOND_PHASE;

	//init mounth
	m_mounthBody[MOUNTH_RIGHT] = nullptr;
	m_mounthBody[MOUNTH_LEFT] = nullptr;
	m_timerMouth = 0;

	//init tears

	m_timerTears = TIMER_RECOVER;
	m_spawnTears = true;

	m_spriteSize["Ground"].x = 2688;
	m_spriteSize["Ground"].y = 530;

	m_spritePos["Ground"].x = -2;
	m_spritePos["Ground"].y = 950;

	m_spriteSize["Balance"].x = 674;
	m_spriteSize["Balance"].y = 1245;

	//m_animPos

	m_spriteSize["BackGround"].x = 2688;
	m_spriteSize["BackGround"].y = 1512;

	m_spriteSize["Mounth"].x = 529;
	m_spriteSize["Mounth"].y = 628;

	m_spriteSize["Flufekey"].x = 206;
	m_spriteSize["Flufekey"].y = 123;

	m_spriteSize["Spawn Flufekey"].x = 338;
	m_spriteSize["Spawn Flufekey"].y = 133;

	m_spriteSize["Eye cry"].x = 492;
	m_spriteSize["Eye cry"].y = 355;

	m_spriteSize["Eye idle"].x = 492;
	m_spriteSize["Eye idle"].y = 140;

	m_spriteSize["Eye close"].x = 492;
	m_spriteSize["Eye close"].y = 144;

	m_spriteSize["Eye dead"].x = 492;
	m_spriteSize["Eye dead"].y = 369;

	m_spritePos["Eye"].x = 1110;
	m_spritePos["Eye"].y = 0;

	m_spriteSize["Tears"].x = 77;
	m_spriteSize["Tears"].y = 78;

	m_spritePos["Pendule"].x = 1350;
	m_spritePos["Pendule"].y = 1000;

	//init animations 

	m_spritesheetGround = LoadSprite("Sprites/Boss/Famine/MounthSpritesheet.png", false);
	m_spritesheetGround.setPosition(m_spritePos["Ground"]);

	m_animGround[0] = InitAnimation(m_spriteSize["Ground"], 5, 4, 0, true);

	// init balance animations

	m_spritesheetBalance[BALANCE_LEFT] = LoadSprite("Sprites/Boss/Famine/TentaculeSpritesheet.png", false);
	m_spritesheetBalance[BALANCE_LEFT].setPosition(sf::Vector2f(PLATFORM_SPRITE_LEFT_X, PLATFORM_SPRITE_Y));
	m_spritesheetBalance[BALANCE_LEFT].setScale(sf::Vector2f(-1, 1));

	m_animBalance[BALANCE_LEFT][TENTACLE_IDLE] = InitAnimation(m_spriteSize["Balance"], 5, 5, 0, true);
	m_animBalance[BALANCE_LEFT][TENTACLE_GRAB] = InitAnimation(m_spriteSize["Balance"], 5, 20, 0, false);
	m_animBalance[BALANCE_LEFT][TENTACLE_UNGRAB] = InitAnimation(m_spriteSize["Balance"], 8, 20, 0, false);
	m_stateAnimBalance[BALANCE_LEFT] = TENTACLE_UNGRAB;

	m_spritesheetBalance[BALANCE_RIGHT] = m_spritesheetBalance[BALANCE_LEFT];
	m_spritesheetBalance[BALANCE_RIGHT].setPosition(sf::Vector2f(PLATFORM_SPRITE_RIGHT_X, PLATFORM_SPRITE_Y));
	m_spritesheetBalance[BALANCE_RIGHT].setScale(sf::Vector2f(1, 1));

	m_animBalance[BALANCE_RIGHT][TENTACLE_IDLE] = m_animBalance[BALANCE_LEFT][TENTACLE_IDLE];
	m_animBalance[BALANCE_RIGHT][TENTACLE_GRAB] = m_animBalance[BALANCE_LEFT][TENTACLE_GRAB];
	m_animBalance[BALANCE_RIGHT][TENTACLE_UNGRAB] = m_animBalance[BALANCE_LEFT][TENTACLE_UNGRAB];
	m_stateAnimBalance[BALANCE_RIGHT] = TENTACLE_UNGRAB;

	m_shaderChainBalance[BALANCE_RIGHT].loadFromFile("Shaders/FamineSecond/Chain.frag", sf::Shader::Fragment);
	m_shaderChainBalance[BALANCE_RIGHT].setUniform("heightSpritesheet", m_spritesheetBalance[BALANCE_RIGHT].getGlobalBounds().height);

	m_shaderChainBalance[BALANCE_LEFT].loadFromFile("Shaders/FamineSecond/Chain.frag", sf::Shader::Fragment);
	m_shaderChainBalance[BALANCE_LEFT].setUniform("heightSpritesheet", m_spritesheetBalance[BALANCE_LEFT].getGlobalBounds().height);

	// end balance animation 

	m_spritesheetFlufekey = LoadSprite("Sprites/Boss/Famine/Mounth.png", false);
	m_spritesheetFlufekey.setOrigin(sf::Vector2f(m_spriteSize["Flufekey"]) / 2.f);

	m_eyeTearSpritesheet = LoadSprite("Sprites/Boss/Famine/Eye.png", false);
	m_eyeTearSpritesheet.setPosition(m_spritePos["Eye"]);

	m_animEye[EYE_TEAR_IDLE] = InitAnimation(m_spriteSize["Eye idle"], 7, 6, 0, true);
	m_animEye[EYE_TEAR_OPEN] = InitAnimation(m_spriteSize["Eye close"], 7, 11, 0, false);
	m_animEye[EYE_TEAR_CLOSE] = InitAnimation(m_spriteSize["Eye close"], 7, 11, 0, false);
	m_animEye[EYE_TEAR_CRY] = InitAnimation(m_spriteSize["Eye cry"], 10, 23, 0, true);
	m_animEye[EYE_TEAR_DEAD] = InitAnimation(m_spriteSize["Eye dead"], 9, 24, 0, false);

	m_stateAnimEye = EYE_TEAR_OPEN;

	m_backGround = LoadSprite("Sprites/Map/Famine/Phase2.png", false);
	m_backgroundSpritesheet = LoadSprite("Sprites/Map/Famine/SpritesheetBackground.png", false);
	m_animBackground[0] = InitAnimation(m_spriteSize["BackGround"], 6, 8, 0, true);

	m_penduleSprite = LoadSprite("Sprites/Boss/Famine/Balance.png", true);
	m_penduleSprite.setPosition(m_spritePos["Pendule"]);

	m_tearSpritesheet = LoadSprite("Sprites/Boss/Famine/Tear.png", false);
	m_tearSpritesheet.setTextureRect(sf::IntRect(0, 0, m_spriteSize["Tears"].x, m_spriteSize["Tears"].y));
	m_tearSpritesheet.setOrigin(sf::Vector2f(m_spriteSize["Tears"]) / 2.f);

	m_spritesheetMounth = LoadSprite("Sprites/Boss/Famine/PlatformMounth.png", false);
	m_spritesheetMounth.setTextureRect(sf::IntRect(0, 0, m_spriteSize["Mounth"].x, m_spriteSize["Mounth"].y));
	m_spritesheetMounth.setOrigin(sf::Vector2f(m_spriteSize["Mounth"]) / 2.f);

	m_animMounth[0] = InitAnimation(m_spriteSize["Mounth"], 12, 28, 0, false);

	m_spawnFlufekeySpritesheet = LoadSprite("Sprites/Boss/Famine/SpawnMounth.png", false);
	m_spawnFlufekeySpritesheet.setTextureRect(sf::IntRect(0, 0, m_spriteSize["Spawn Flufekey"].x, m_spriteSize["Spawn Flufekey"].y));
	m_spawnFlufekeySpritesheet.setOrigin(sf::Vector2f(m_spriteSize["Spawn Flufekey"]) / 2.f);

	m_animSpawnFlufekey[0] = InitAnimation(m_spriteSize["Spawn Flufekey"], 12, 16, 0, false);
	m_flufekeySpawn = false;

}


void BossFamineSecondPhase::Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData)
{

	if (m_status != BOSS_DEAD)
	{
		m_globalTimer -= _dt;
		m_timerSpawnFlufekey += _dt;

		Animation(m_spritesheetGround, m_animGround, 0, _dt);
		Animation(m_eyeTearSpritesheet, m_animEye, m_stateAnimEye, _dt);
		Animation(m_spritesheetMounth, m_animMounth, 0, _dt, 7406);
		Animation(m_spawnFlufekeySpritesheet, m_animSpawnFlufekey, 0, _dt);
		Animation(m_backgroundSpritesheet, m_animBackground, 0, _dt, 10752);

		Animation(m_spritesheetBalance[BALANCE_LEFT], m_animBalance[BALANCE_LEFT], m_stateAnimBalance[BALANCE_LEFT], _dt);
		Animation(m_spritesheetBalance[BALANCE_RIGHT], m_animBalance[BALANCE_RIGHT], m_stateAnimBalance[BALANCE_RIGHT], _dt);

		m_shaderChainBalance[BALANCE_LEFT].setUniform("heightAnim", (m_spriteSize["Balance"].y * m_stateAnimBalance[BALANCE_LEFT]));
		m_shaderChainBalance[BALANCE_RIGHT].setUniform("heightAnim", (m_spriteSize["Balance"].y * m_stateAnimBalance[BALANCE_RIGHT]));

		for (auto& element : m_animFlufekey)
		{
			element->timeAnim += _dt;
		}

		for (auto& element : m_animTear)
		{
			element->timeAnim += _dt;
		}

		if (m_status == RECOVER && m_globalTimer <= 0)
		{
			m_status = rand() % 2;

			CreateTentacle(m_status, _world);
			m_tentacleHp = TENTACLE_HP;
			m_canHit = false;

			m_animBalance[m_status]->timeAnim = 0;
			m_stateAnimBalance[m_status] = TENTACLE_GRAB;

			m_animEye->timeAnim = 0;

		}

		if (m_status != RECOVER)
		{
			float frameTimeTentacle = 1.f / m_animBalance[m_status][TENTACLE_GRAB].framerate;
			int frameNbTentacle = m_animBalance[m_status]->timeAnim / frameTimeTentacle;

			if (frameNbTentacle >= m_animBalance[m_status][TENTACLE_GRAB].frameNb)
			{
				m_animBalance[m_status]->timeAnim = 0;
				m_stateAnimBalance[m_status] = TENTACLE_IDLE;
			}

			m_stateAnimEye = EYE_TEAR_CLOSE;
			PlatformMove(DOWN, m_status);
			m_timerMouth += _dt;

		}
		else
		{
			float frameTimeEyeOpen = 1.f / m_animEye[EYE_TEAR_OPEN].framerate;
			int frameNbEyeOpen = m_animEye->timeAnim / frameTimeEyeOpen;

			PlatformMove(UP, PLATFORM_RIGHT);
			PlatformMove(UP, PLATFORM_LEFT);

			if (m_stateAnimEye == EYE_TEAR_CRY)
			{
				float frameTime = 1.f / m_animEye[EYE_TEAR_CRY].framerate;
				int frameNb = m_animEye->timeAnim / frameTime;

				m_timerMouth = 0;
				m_timerTears += _dt;

				if (frameNb >= m_animEye[EYE_TEAR_CRY].frameNb - 1)
				{
					if (m_spawnTears)
					{
						float posYTearsOnMeters = Screen2World(sf::Vector2f(0, m_eyeTearSpritesheet.getGlobalBounds().height - 20)).y;

						TearsHit(_world, sf::Vector2f(m_body->GetPosition().x, posYTearsOnMeters));

						m_spawnTears = false;
					}
				}
				else
				{
					m_spawnTears = true;
				}
			}
			else if (frameNbEyeOpen >= m_animEye[EYE_TEAR_OPEN].frameNb)
			{
				m_animEye->timeAnim = 0;
				m_stateAnimEye = EYE_TEAR_CRY;
			}
		}

		CreateMount(_world);

		CreateFlufekey(_world);
		FlufekeyUpdate(_dt);

		if (m_hp <= 0)
		{
			//ClearFight(_world);
			m_status = BOSS_DEAD;

			m_hp = 100;
			m_stateAnimEye = EYE_TEAR_DEAD;
			m_animEye->timeAnim = 0;
		}
	}
	else
	{
		Animation(m_backgroundSpritesheet, m_animBackground, 0, _dt, 10752);
		Animation(m_spritesheetGround, m_animGround, 0, _dt);
		Animation(m_eyeTearSpritesheet, m_animEye, m_stateAnimEye, _dt);

		if (GetActualyFrame(m_animEye, m_stateAnimEye) >= m_animEye[m_stateAnimEye].frameNb && m_destroyWall)
		{
			GetBodyOfMap(_world, "Roof midle")->GetFixtureList()->SetSensor(true);
			((UserData*)GetBodyOfMap(_world, "Roof midle")->GetFixtureList()->GetUserData().pointer)->interruptGrapin = false;
			m_destroyWall = false;
		}

		if (m_playerPos.y >= 2.25)
		{
			m_hp = 0;
		}
		else
		{
			m_hp = 100;
		}

	}
}


void BossFamineSecondPhase::PlatformMove(int _dir, int _platformSelected)
{
	b2Vec2 platformPos = m_platforms[_platformSelected]->GetPosition();
	sf::Vector2f posSprite;
	float distChain;

	platformPos.y += _dir / 10.f;


	if (platformPos.y > PLATFORM_MIN_Y && platformPos.y < PLATFORM_MAX_Y && (m_stateAnimBalance[_platformSelected] == TENTACLE_IDLE || m_stateAnimBalance[_platformSelected] == TENTACLE_UNGRAB))
	{
		m_platforms[_platformSelected]->SetLinearVelocity(b2Vec2(0, 2 * _dir));
	}
	else
	{
		m_platforms[_platformSelected]->SetLinearVelocity(b2Vec2(0, 0));
	}

	posSprite.x = m_spritesheetBalance[_platformSelected].getPosition().x;
	posSprite.y = 330 + (World2Screen(m_platforms[_platformSelected]->GetPosition()).y - World2Screen(b2Vec2(m_platforms[_platformSelected]->GetPosition().x, PLATFORM_MAX_Y)).y);

	distChain = (m_spritesheetBalance[_platformSelected].getGlobalBounds().top + m_spriteSize["Balance"].y) - 536;
	distChain = m_spriteSize["Balance"].y - distChain;

	m_shaderChainBalance[_platformSelected].setUniform("distChainY", distChain);
	m_spritesheetBalance[_platformSelected].setPosition(posSprite);
}


void BossFamineSecondPhase::CreateTentacle(int _platform, b2World* _world)
{

	b2PolygonShape secondTentacleShape;
	int32 nbPoints = 4;
	b2FixtureDef fixtureDef;
	b2Vec2* pointsTentacle = new b2Vec2[nbPoints];

	if (_platform == BALANCE_RIGHT)
	{
		pointsTentacle[0] = { 0, 0 };
		pointsTentacle[1] = { TENTACLE_SIZE_X, 0 };
		pointsTentacle[2] = { TENTACLE_SIZE_X, TENTACLE_SIZE_Y };
		pointsTentacle[3] = { 0, TENTACLE_SIZE_Y };
		secondTentacleShape.Set(pointsTentacle, nbPoints);

		fixtureDef.userData.pointer = (uintptr_t) new std::string("Tentacle");
		fixtureDef.shape = &secondTentacleShape;
		fixtureDef.density = 0;
		fixtureDef.friction = 0;

		m_platforms[_platform]->CreateFixture(&fixtureDef)->SetSensor(true);
	}
	//create second hitbox
	if (_platform == BALANCE_LEFT)
	{
		pointsTentacle[0] = { PLATFORM_WIDTH - TENTACLE_SIZE_X, 0 };
		pointsTentacle[1] = { PLATFORM_WIDTH, 0 };
		pointsTentacle[2] = { PLATFORM_WIDTH, TENTACLE_SIZE_Y };
		pointsTentacle[3] = { PLATFORM_WIDTH - TENTACLE_SIZE_X, TENTACLE_SIZE_Y };
		secondTentacleShape.Set(pointsTentacle, nbPoints);

		fixtureDef.userData.pointer = (uintptr_t) new std::string("Tentacle");
		fixtureDef.shape = &secondTentacleShape;
		fixtureDef.density = 0;
		fixtureDef.friction = 0;

		m_platforms[_platform]->CreateFixture(&fixtureDef)->SetSensor(true);
	}
	// end create second hitbox
}

void BossFamineSecondPhase::TentacleTakeDamage()
{
	m_tentacleHp--;

	if (m_tentacleHp <= 0)
	{
		m_platforms[m_status]->DestroyFixture(m_platforms[m_status]->GetFixtureList());

		m_animBalance[m_status]->timeAnim = 0;
		m_stateAnimBalance[m_status] = TENTACLE_UNGRAB;

		m_globalTimer = TIMER_RECOVER;
		m_status = RECOVER;
		m_canHit = true;

		m_animEye->timeAnim = 0;
		m_stateAnimEye = EYE_TEAR_OPEN;
	}
}

void BossFamineSecondPhase::CreateMount(b2World* _world)
{

	float frameTimeMounth = 1.f / m_animMounth[0].framerate;
	int frameNbMounth = m_animMounth[0].timeAnim / frameTimeMounth;

	if (m_timerMouth > MOUNTH_DISPAWN)
	{
		if (m_mounthBody[MOUNTH_LEFT] != nullptr && frameNbMounth > 25)
		{
			_world->DestroyBody(m_mounthBody[MOUNTH_LEFT]);
			m_mounthBody[MOUNTH_LEFT] = nullptr;
		}
		else if (m_mounthBody[MOUNTH_RIGHT] != nullptr && frameNbMounth > 25)
		{
			_world->DestroyBody(m_mounthBody[MOUNTH_RIGHT]);
			m_mounthBody[MOUNTH_RIGHT] = nullptr;
		}
	}

	if (m_timerMouth > TIMER_MOUNTH)
	{
		if (m_status == MOUNTH_RIGHT)
		{

			sf::Vector2f posHitBoxMounth = sf::Vector2f(m_platforms[m_status]->GetPosition().x - MOUNTH_OFFSET_SPAWN_X, m_platforms[m_status]->GetPosition().y + MOUNTH_OFFSET_SPAWN_Y);
			m_spritesheetMounth.setScale(sf::Vector2f(1, 1));
			m_spritesheetMounth.setPosition(sf::Vector2f(World2ScreenVec(posHitBoxMounth).x - 80, World2ScreenVec(posHitBoxMounth).y + 40));
			m_animMounth->timeAnim = 0;
		}

		//Mounth Left create
		if (m_status == MOUNTH_LEFT)
		{
			sf::Vector2f posHitBoxMounth = sf::Vector2f(m_platforms[m_status]->GetPosition().x + PLATFORM_WIDTH + MOUNTH_OFFSET_SPAWN_X, m_platforms[m_status]->GetPosition().y + MOUNTH_OFFSET_SPAWN_Y);

			m_spritesheetMounth.setScale(sf::Vector2f(-1, 1));
			m_spritesheetMounth.setPosition(sf::Vector2f(World2ScreenVec(posHitBoxMounth).x + 80, World2ScreenVec(posHitBoxMounth).y + 40));
			m_animMounth->timeAnim = 0;
		}
		m_timerMouth = 0;
	}
	else
	{
		B2Dinfo mounthInfo;
		//MounthRight create

		mounthInfo.type = b2_kinematicBody;
		mounthInfo.shape = CIRCLE;
		mounthInfo.userData.fixtureName = "Mounth";
		mounthInfo.radius = 1.25;
		mounthInfo.density = 20;
		mounthInfo.friction = 1;
		mounthInfo.fixedRotation = true;


		if (m_mounthBody[MOUNTH_LEFT] == nullptr && frameNbMounth <= 24 && frameNbMounth >= 23 && m_status == MOUNTH_LEFT)
		{
			mounthInfo.pos = sf::Vector2f(m_platforms[m_status]->GetPosition().x + PLATFORM_WIDTH + MOUNTH_OFFSET_SPAWN_X, m_platforms[m_status]->GetPosition().y + MOUNTH_OFFSET_SPAWN_Y);
			m_mounthBody[MOUNTH_LEFT] = CreateBodyB2D(_world, mounthInfo);
			m_mounthBody[MOUNTH_LEFT]->GetFixtureList()[0].SetSensor(true);


		}
		else if (m_mounthBody[MOUNTH_RIGHT] == nullptr && frameNbMounth <= 24 && frameNbMounth >= 23 && m_status == MOUNTH_RIGHT)
		{
			mounthInfo.pos = sf::Vector2f(m_platforms[m_status]->GetPosition().x - MOUNTH_OFFSET_SPAWN_X, m_platforms[m_status]->GetPosition().y + MOUNTH_OFFSET_SPAWN_Y);
			m_mounthBody[MOUNTH_RIGHT] = CreateBodyB2D(_world, mounthInfo);
			m_mounthBody[MOUNTH_RIGHT]->GetFixtureList()[0].SetSensor(true);
		}

	}
}

void BossFamineSecondPhase::TearsHit(b2World* _world, sf::Vector2f _pos)
{
	Anim* tearsAnim;
	B2Dinfo tearsInfo;

	tearsAnim = new Anim;

	tearsInfo.type = b2_dynamicBody;
	tearsInfo.shape = CIRCLE;
	tearsInfo.userData.fixtureName = "TearsGround";
	tearsInfo.userData.tearsGroundVec = &m_tearsGroundVec;
	tearsInfo.userData.animTears = &m_animTear;
	tearsInfo.radius = 0.25;
	tearsInfo.density = 1;
	tearsInfo.friction = 1;
	tearsInfo.fixedRotation = true;

	_pos.y += tearsInfo.radius;
	tearsInfo.pos = _pos;

	m_tearsGroundVec.push_back(CreateBodyB2D(_world, tearsInfo));
	m_tearsGroundVec[m_tearsGroundVec.size() - 1]->GetFixtureList()[0].SetSensor(true);
	m_tearsGroundVec[m_tearsGroundVec.size() - 1]->SetGravityScale(0);
	m_tearsGroundVec[m_tearsGroundVec.size() - 1]->SetLinearVelocity(b2Vec2(TEARS_SPEED, 0));

	m_tearsGroundVec.push_back(CreateBodyB2D(_world, tearsInfo));
	m_tearsGroundVec[m_tearsGroundVec.size() - 1]->GetFixtureList()[0].SetSensor(true);
	m_tearsGroundVec[m_tearsGroundVec.size() - 1]->SetGravityScale(0);
	m_tearsGroundVec[m_tearsGroundVec.size() - 1]->SetLinearVelocity(b2Vec2(-TEARS_SPEED, 0));

	tearsAnim[0] = InitAnimation(sf::Vector2i(77, 78), 2, 4, 0, true);

	m_animTear.push_back(tearsAnim);
	m_animTear.push_back(tearsAnim);

}

void BossFamineSecondPhase::FlufekeyUpdate(float _dt)
{

	for (auto& flufekey : m_flufekeyVec)
	{
		b2Vec2 pos = flufekey.body->GetPosition();
		float dir = atan2((m_playerPos.y) - pos.y, (m_playerPos.x) - pos.x);
		sf::Vector2f newVelocity;
		float normVelocity;

		flufekey.timerDash += _dt;

		newVelocity.x = flufekey.velocityBody.x + FLUFEKEY_ACCELERATION * cos(dir) * _dt;
		newVelocity.y = flufekey.velocityBody.y + FLUFEKEY_ACCELERATION * sin(dir) * _dt;

		//Schearch the speed
		normVelocity = sqrtf(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);

		if (normVelocity < FLUFEKEY_MAX_SPEED)
		{
			flufekey.velocityBody.x = newVelocity.x;
			flufekey.velocityBody.y = newVelocity.y;
		}
		else
		{
			// Allows you to change direction without exceeding the maximum speed
			flufekey.velocityBody *= FLUFEKEY_MAX_SPEED / normVelocity;
		}

		if (flufekey.timerDash >= 5)
		{
			flufekey.velocityBody.x = FLUFEKEY_MAX_SPEED * cos(dir);
			flufekey.velocityBody.y = FLUFEKEY_MAX_SPEED * sin(dir);
			flufekey.timerDash = 0;
		}


		flufekey.body->SetLinearVelocity(flufekey.velocityBody);
	}
}

void BossFamineSecondPhase::CreateFlufekey(b2World* _world)
{
	// x: -2.1 y: -0 left top corner
	// x: -2.1 y: -23 left down corner
	// x: 44 y: -0 right top corner
	// x: 44 y: -23 right down corner


	float frameTimeFlufekeySpawn = 1.f / m_animSpawnFlufekey[0].framerate;
	int frameNbFlufekeySpawn = m_animSpawnFlufekey[0].timeAnim / frameTimeFlufekeySpawn;

	if (frameNbFlufekeySpawn >= m_animSpawnFlufekey->frameNb && m_flufekeySpawn)
	{
		Anim* flufekeyAnim;
		int randSpawn = rand() % 4;
		B2Dinfo infoFlufekey;
		Flufekey flufekey;

		flufekeyAnim = new Anim;
		infoFlufekey.pos = Screen2WorldVec(sf::Vector2f(m_spawnFlufekeySpritesheet.getPosition().x + 35, m_spawnFlufekeySpritesheet.getPosition().y));

		infoFlufekey.type = b2_kinematicBody;
		infoFlufekey.shape = CIRCLE;
		infoFlufekey.userData.fixtureName = "Flufekey";
		infoFlufekey.userData.flufekeyVec = &m_flufekeyVec;
		infoFlufekey.userData.animFlufekey = &m_animFlufekey;
		infoFlufekey.userData.interruptGrapin = false;
		infoFlufekey.radius = 0.4;
		infoFlufekey.density = 1;
		infoFlufekey.friction = 1;
		infoFlufekey.fixedRotation = true;

		flufekey.body = CreateBodyB2D(_world, infoFlufekey);
		flufekey.body->GetFixtureList()[0].SetSensor(true);

		flufekey.velocityBody = { -FLUFEKEY_MAX_SPEED, 0 };
		flufekey.timerDash = 0;

		flufekeyAnim[0] = InitAnimation(m_spriteSize["Flufekey"], 20, 40, 0, true);

		m_flufekeyVec.push_back(flufekey);
		m_animFlufekey.push_back(flufekeyAnim);

		m_flufekeySpawn = false;

	}

	if (m_timerSpawnFlufekey > TIMER_SPAWN_FLUFEKEY - 5)
	{
		sf::Vector2f pos;
		pos.x = Meter2Pixel(-2.1 + rand() % 44);
		pos.y = Meter2Pixel(-(rand() % 23));

		m_spawnFlufekeySpritesheet.setPosition(pos);
		m_flufekeySpawn = true;

		m_animSpawnFlufekey[0].timeAnim = 0;
		m_timerSpawnFlufekey = 0;
	}


}

void BossFamineSecondPhase::Display(sf::RenderWindow& _window)
{
	_window.draw(m_backgroundSpritesheet);
	_window.draw(m_backGround);

	_window.draw(m_penduleSprite);

	_window.draw(m_spritesheetBalance[BALANCE_RIGHT], &m_shaderChainBalance[BALANCE_RIGHT]);
	_window.draw(m_spritesheetBalance[BALANCE_LEFT], &m_shaderChainBalance[BALANCE_LEFT]);
	_window.draw(m_eyeTearSpritesheet);

	for (int i = 0; i < m_tearsGroundVec.size(); i++)
	{
		Animation(m_tearSpritesheet, m_animTear[i], 0, 0);
		BlitSprite(m_tearSpritesheet, World2Screen(m_tearsGroundVec[i]->GetPosition()), 0, _window);
	}

	if (m_status != RECOVER)
	{
		_window.draw(m_spritesheetMounth);
	}

	if (m_flufekeySpawn)
	{

		_window.draw(m_spawnFlufekeySpritesheet);

	}

	for (int i = 0; i < m_flufekeyVec.size(); i++)
	{
		sf::Vector2f posFlu = World2Screen(m_flufekeyVec[i].body->GetPosition());
		sf::Vector2f posPla = World2Screen(m_playerPos);

		float angle = atan2(-m_flufekeyVec[i].velocityBody.y, m_flufekeyVec[i].velocityBody.x);

		if (angle < M_PI_2 && angle > -M_PI_2)
		{
			m_spritesheetFlufekey.setScale(sf::Vector2f(1, -1));
		}
		else
		{
			m_spritesheetFlufekey.setScale(sf::Vector2f(1, 1));
		}

		Animation(m_spritesheetFlufekey, m_animFlufekey[i], 0, 0);
		BlitSprite(m_spritesheetFlufekey, World2Screen(m_flufekeyVec[i].body->GetPosition()), RadToDeg(angle) + 180, _window);
	}


}

void BossFamineSecondPhase::DisplayFront(sf::RenderWindow& _window)
{
	if (m_status == BOSS_DEAD)
	{
		_window.draw(m_eyeTearSpritesheet);
	}

	_window.draw(m_spritesheetGround);
}

void BossFamineSecondPhase::ClearFight(b2World* _world)
{
	for (int i = 0; i < 2; i++)
	{
		if (m_platforms[i] != nullptr)
		{
			_world->DestroyBody(m_platforms[i]);
		}
		m_platforms[i] = nullptr;

		if (m_mounthBody[i] != nullptr)
		{
			_world->DestroyBody(m_mounthBody[i]);
		}
		m_mounthBody[i] = nullptr;

	}


	for (auto& element : m_flufekeyVec)
	{
		if (element.body != nullptr)
		{
			_world->DestroyBody(element.body);
		}
		element.body = nullptr;
	}

	m_flufekeyVec.clear();

	for (auto& element : m_tearsGroundVec)
	{
		if (element != nullptr)
		{
			_world->DestroyBody(element);
		}
		element = nullptr;
	}

	m_tearsGroundVec.clear();

}