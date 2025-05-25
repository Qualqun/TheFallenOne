#include "BossPandemic.h"

#define MIST_X_OFFSET -500
#define SKULL_DETECT_X 6

#define TIME_RECOVER 2
#define ACCELERATION 5

#define SPRITESHEET_SPAWN_PANDEMIC_X 9100
#define TIME_SPAWN_WAVES 0.25

#define SPACE_INTER_PIC 4
#define PIC_TIME_ATTACK 0.5
#define PIC_TIME_DISPAWN 1

#define ANIM_SIZE_MAX_PIC_AND_TP 8724
#define ANIM_SIZE_MAX_DEFAULT 16000
#define ANIM_SIZE_MAX_RAT_BALL 12420
#define ANIM_SIZE_MAX_BIG_RAT 10808

#define TIME_RAT_SHOOT 0.15
#define SPEED_RAT_SHOOT 12.5
#define SPEED_SPEAR 12.5

enum StatusAnimJump
{
	START_JUMP,
	END_JUMP,
	RE_SPAWN
};

enum SkullAnim
{
	PRESS_E,
	ACTIVATE
};

enum PicStatus
{
	SHOWING,
	DAMAGE,
	NO_DAMAGE,
};

enum ShootStatus
{
	START_SHOOT,
	END_SHOOT
};

enum RatBallStatus
{
	START_PHASE,
	END_PHASE
};

enum RatBallDirections
{
	RAT_BALL_LEFT = 1,
	RAT_BALL_RIGHT = -1
};

BossPandemicData::BossPandemicData()
{
	m_stateBoss = OFF;
	m_paterns = NO_PATERN;

	m_globalTimer = TIME_RECOVER;

	m_hp = 100;
	m_stateShootPatern = START_SHOOT;
	m_spearBody = nullptr;
	m_playerEated = false;
	m_showAttacks = 0;
	m_timerScream = 0;
	//Pos x : 550 Pos y : 813


	for (int i = 1; i <= TOT_LAYER_BACKGROUND; i++)
	{
		std::string pathLayerImage = "Sprites/Map/Pandemic/Layer" + std::to_string(i) + ".png";

		m_backgroundLayer[i - 1] = LoadSprite(pathLayerImage, false);
		m_backgroundLayer[i - 1].setPosition(sf::Vector2f(-1920 * 2, 0));

	}

	
	m_gamePhase = PANDEMINE_FIRST_PHASE;

	//Mist
	m_spritesPandemic[MIST_SPRITE] = LoadSprite("Sprites/Map/Pandemic/MistAnimeBg.png", false);
	m_mistAnim[0] = InitAnimation(sf::Vector2i(1920, 535), 12, 38, 0, true);

	m_mistAnimBack[0] = InitAnimation(sf::Vector2i(1920, 535), 8, 38, 0, true);
	m_mistAnimBack[0].timeAnim = 1.5;

	// Skull

	m_spriteSize["Skull"] = sf::Vector2i(418, 340);
	m_spritePos["Skull"] = sf::Vector2f(550, 700);

	m_spritesPandemic[SKULL_SPRITE] = LoadSprite("Sprites/Map/Pandemic/Skull.png", false);
	m_spritesPandemic[SKULL_SPRITE].setTextureRect(sf::IntRect(0, 0, m_spriteSize["Skull"].x, m_spriteSize["Skull"].y));
	m_spritesPandemic[SKULL_SPRITE].setOrigin(sf::Vector2f(m_spriteSize["Skull"]) / 2.f);
	m_spritesPandemic[SKULL_SPRITE].setPosition(m_spritePos["Skull"]);


	m_skullAnim[PRESS_E] = InitAnimation(sf::Vector2i(418, 340), 18, 15, 0, false);
	m_skullAnim[ACTIVATE] = InitAnimation(sf::Vector2i(418, 340), 12/*12*/, 29, 0, false);

	m_statusAnimSkull = PRESS_E;

	//Spawn boss

	m_spriteSize["Spawn Boss"] = sf::Vector2i(910, 505);
	m_spritePos["Spawn Boss"] = sf::Vector2f(1500, 720);

	m_spritesPandemic[SPAWN_BOSS_SPRITE] = LoadSprite("Sprites/Boss/Pandemic/BossSpawnSpriteSheet.png", false);
	m_spritesPandemic[SPAWN_BOSS_SPRITE].setTextureRect(sf::IntRect(0, 0, m_spriteSize["Spawn Boss"].x, m_spriteSize["Spawn Boss"].y));
	m_spritesPandemic[SPAWN_BOSS_SPRITE].setOrigin(sf::Vector2f(m_spriteSize["Spawn Boss"]) / 2.f);
	m_spritesPandemic[SPAWN_BOSS_SPRITE].setPosition(m_spritePos["Spawn Boss"]);

	m_spawnBossAnim[0] = InitAnimation(m_spriteSize["Spawn Boss"], 12/*12*/, 110, 0, false);

	//Geant Rat
	m_spriteSize["Geant Rat"] = sf::Vector2i(1544, 719);

	m_spritesPandemic[BIG_RAT_SPRITE] = LoadSprite("Sprites/Boss/Pandemic/RatGeant.png", false);
	m_spritesPandemic[BIG_RAT_SPRITE].setTextureRect(sf::IntRect(0, 0, m_spriteSize["Geant Rat"].x, m_spriteSize["Geant Rat"].y));
	m_spritesPandemic[BIG_RAT_SPRITE].setOrigin(sf::Vector2f(m_spriteSize["Geant Rat"].x / 2, 0));

	m_spritesPandemic[BIG_RAT_SPRITE].setPosition(sf::Vector2f(-5000, 0));

	m_bigRatAnim[0] = InitAnimation(m_spriteSize["Geant Rat"], 9, 14, 0, false);

	// Boss
	// Rat shoot
	m_spriteSize["Rat Shoot"] = sf::Vector2i(111, 56);

	m_spritesPandemic[RAT_SHOOT] = LoadSprite("Sprites/Boss/Pandemic/RatShootSpritesheet.png", false);
	m_spritesPandemic[RAT_SHOOT].setTextureRect(sf::IntRect(0, 0, m_spriteSize["Rat Shoot"].x, m_spriteSize["Rat Shoot"].y));
	m_spritesPandemic[RAT_SHOOT].setOrigin(sf::Vector2f(m_spriteSize["Rat Shoot"]) / 2.f);


	// Rat ball
	m_spriteSize["Rat Ball"] = sf::Vector2i(828, 592);

	m_spritesPandemic[RAT_BALL_SPRITE] = LoadSprite("Sprites/Boss/Pandemic/RatBall.png", false);
	m_spritesPandemic[RAT_BALL_SPRITE].setTextureRect(sf::IntRect(0, 0, m_spriteSize["Rat Ball"].x, m_spriteSize["Rat Ball"].y));
	m_spritesPandemic[RAT_BALL_SPRITE].setOrigin(sf::Vector2f(m_spriteSize["Rat Ball"].x / 2, 0));

	m_ratBallAnim[0] = InitAnimation(m_spriteSize["Rat Ball"], 12, 18 /*43*/, 0, false);

	m_spritesPandemic[SPEAR] = LoadSprite("Sprites/Boss/Pandemic/Spear.png", false);
	m_spritesPandemic[SPEAR].setOrigin(sf::Vector2f(m_spritesPandemic[SPEAR].getGlobalBounds().width / 2, 0));

	// ALL ANIM BOSS
	m_spriteSize["Boss Idle"] = sf::Vector2i(727, 507);
	m_spriteSize["Boss Pic"] = sf::Vector2i(727, 507);
	m_spriteSize["Boss Jump start"] = sf::Vector2i(814, 645);
	m_spriteSize["Boss Jump end"] = sf::Vector2i(678, 1000);
	m_spriteSize["Boss Rat shoot"] = sf::Vector2i(727, 598);
	m_spriteSize["Boss Tp"] = sf::Vector2i(727, 507);
	m_spriteSize["Boss Dead"] = sf::Vector2i(727, 507);

	m_spritesPandemic[BOSS_SPRITE] = LoadSprite("Sprites/Boss/Pandemic/SpritesheetPandemic.png", false);
	m_spritesPandemic[BOSS_SPRITE].setOrigin(sf::Vector2f(m_spriteSize["Boss Idle"].x / 2, 0));

	m_bossAnim[IDLE_ANIM] = InitAnimation(m_spriteSize["Boss Idle"], 10, 8, 0, true);
	m_bossAnim[PIC_ANIM] = InitAnimation(m_spriteSize["Boss Pic"], 17, 22, 0, false);
	m_bossAnim[PIC_ANIM_SECOND_LINE] = InitAnimation(m_spriteSize["Boss Pic"], 0, 0, 0, false);
	m_bossAnim[JUMP_START_ANIM] = InitAnimation(m_spriteSize["Boss Jump start"], 10, 16, 0, false);
	m_bossAnim[JUMP_END_ANIM] = InitAnimation(m_spriteSize["Boss Jump end"], 10, 14, 0, false);
	m_bossAnim[RAT_SHOOT_ANIM] = InitAnimation(m_spriteSize["Boss Rat shoot"], 13, 13, 0, false);
	m_bossAnim[TP_ANIM] = InitAnimation(m_spriteSize["Boss Tp"], 11, 24, 0, false);
	m_bossAnim[TP_ANIM_SECOND_LINE] = InitAnimation(m_spriteSize["Boss Tp"], 0, 0, 0, false);
	m_bossAnim[DEAD_ANIM] = InitAnimation(m_spriteSize["Boss Dead"], 7, 6, 0, false);
	m_bossAnim[DEAD_BOUCLE] = InitAnimation(m_spriteSize["Boss Dead"], 7, 6, 0, true);

	m_sizeMaxAnimBoss = ANIM_SIZE_MAX_DEFAULT;
	m_animState = IDLE_ANIM;
	m_turnSpriteToPlayer = { 1,1 };
	m_offSetAnim = { 0, 0 };
	m_statusAnimJump = START_JUMP;

	m_spriteSize["Slash"] = sf::Vector2i(533, 361);

	m_spritesPandemic[SLASH] = LoadSprite("Sprites/Boss/Pandemic/SpriteSheetSlash.png", false);
	m_spritesPandemic[SLASH].setTextureRect(sf::IntRect(0, 0, sf::Vector2i(533, 361).x, sf::Vector2i(533, 361).y));
	m_spritesPandemic[SLASH].setOrigin(sf::Vector2f(m_spriteSize["Slash"].x / 2, 0));

	m_slashAnim[0] = InitAnimation(m_spriteSize["Slash"], 12, 7, 0, true);

	m_spriteSize["Pic"] = sf::Vector2i(129, 808);
	

	m_spritesPandemic[PIC_SPRITE] = LoadSprite("Sprites/Boss/Pandemic/PicSpriteSheet.png", false);
	m_spritesPandemic[PIC_SPRITE].setTextureRect(sf::IntRect(0, 0, m_spriteSize["Pic"].x, m_spriteSize["Pic"].y));
	m_spritesPandemic[PIC_SPRITE].setOrigin(sf::Vector2f(m_spriteSize["Pic"].x / 4, 0));

	m_picAnim[0] = InitAnimation(sf::Vector2i(m_spriteSize["Pic"].x, 1850), 10, 19 /*19 42*/, 0, false);

	m_maxHp = m_hp;
}

void BossPandemicData::Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData)
{
	m_mistAnim[0].timeAnim += _dt;
	m_mistAnimBack[0].timeAnim += _dt;

	if (m_stateBoss == OFF)
	{
		float frameTimeSkull = 1.f / m_skullAnim[ACTIVATE].framerate;
		int frameNbSkull = m_skullAnim->timeAnim / frameTimeSkull;

		if (frameNbSkull >= m_skullAnim[ACTIVATE].frameNb && m_statusAnimSkull == ACTIVATE)
		{
			m_stateBoss = SPAWN_BOSS;
			_soundData->UseSound(SOURCE_BOSS_PANDEMIC, SPAWN_BOSS_SOUND);
		}

		if (abs(m_playerPos.x - Pixel2Meter(m_spritesPandemic[SKULL_SPRITE].getPosition().x)) <= SKULL_DETECT_X || m_statusAnimSkull == ACTIVATE)
		{
			m_skullAnim->timeAnim += _dt;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && m_statusAnimSkull != ACTIVATE)
			{
				m_statusAnimSkull = ACTIVATE;
				_soundData->UseSound(SOURCE_BOSS_PANDEMIC, LAUNCH_BOSS_SOUND);
				m_skullAnim->timeAnim = 0;
			}
		}
		else
		{
			m_skullAnim->timeAnim -= _dt;

			if (m_skullAnim->timeAnim <= 0)
			{
				m_skullAnim->timeAnim = 0;
			}
		}


	}
	else if (m_stateBoss == SPAWN_BOSS)
	{
		float frameTimeSpawnBoss = 1.f / m_spawnBossAnim->framerate;
		int frameNbSpawnBoss = m_spawnBossAnim->timeAnim / frameTimeSpawnBoss;
		_camData->SetCameraFollowPlayer(false);
		_camData->InterpolCameraTo(sf::Vector2f(1200, _camData->GetPosition().y), 1);

		Animation(m_spritesPandemic[SPAWN_BOSS_SPRITE], m_spawnBossAnim, 0, _dt, SPRITESHEET_SPAWN_PANDEMIC_X);

		if (GetActualyFrame(m_spawnBossAnim) >= 88)
		{
			m_timerScream -= _dt;

			if (m_timerScream <= 0)
			{
				_particleData->CreateBossScreamParticle(sf::Vector2f{ 1244, 712 }, 0.5, 0.3);
				m_timerScream = TIME_SPAWN_WAVES;
			}
		}

		if (frameNbSpawnBoss >= m_spawnBossAnim->frameNb)
		{
			_camData->SetCameraFollowPlayer(true);
			_camData->SetCameraRangeX(sf::Vector2f(Meter2Pixel(GetBodyOfMap(_world, "StartBoss Wall")->GetPosition().x), _camData->GetCameraRangeX().y));
			GetBodyOfMap(_world, "StartBoss Wall")->GetFixtureList()[0].SetSensor(false);

			SpawnBoss(_world);

			m_stateBoss = START_FIGHT;
			_soundData->StartBackgroundMusic(MUSIC_PANDEMIC);
		}


	}
	else if (m_stateBoss == START_FIGHT)
	{

		m_spritesPandemic[BOSS_SPRITE].setScale(m_turnSpriteToPlayer);
		m_globalTimer -= _dt;

		if (m_globalTimer <= 0 && m_paterns == NO_PATERN)
		{

			_soundData->UseSound(SOURCE_BOSS_PANDEMIC, ATTACK_SOUND_1 + rand() % 5);

			m_showAttacks++;

			if (m_showAttacks >= TOT_PATERN)
			{
				m_showAttacks = 1;
			}

			m_paterns = m_showAttacks /*1 + rand() % PIC*/;


			switch (m_paterns)
			{
			case CHARGE:
				m_aimPlayer = START_AIM;

				m_statusAnimJump = START_JUMP;
				m_animState = JUMP_START_ANIM;
				m_bossAnim->timeAnim = 0;

				m_body->SetLinearVelocity(b2Vec2(0, 0));
				break;

			case PIC_PATERN:
				CratePic(_world);
				m_picStatus = SHOWING;
				m_timerPic = 0;

				break;
			case SHOOT_PATERN:
				m_stateShootPatern = START_SHOOT;
				m_animState = RAT_SHOOT_ANIM;
				m_bossAnim->timeAnim = 0;
				m_nbRatShoot = 0;
				break;
			case RAT_BALL:
				CreateRatBall(_world);
				CreateSpear(_world);

				m_animState = TP_ANIM;
				m_bossAnim->timeAnim = 0;
				m_sizeMaxAnimBoss = ANIM_SIZE_MAX_PIC_AND_TP;
				m_statusRatBall = START_PHASE;

				break;
			default:
				break;
			}

		}
		else if (m_paterns == NO_PATERN)
		{
			m_bossAnim->timeAnim += _dt;
			TurnToPlayer();
		}

		for (auto& ratShoot : m_ratShoot)
		{
			ratShoot.anim->timeAnim += _dt;
		}

		switch (m_paterns)
		{
		case CHARGE:
			ChargePatern(_world, _dt);
			break;
		case PIC_PATERN:
			PicPatern(_world, _dt);
			break;
		case SHOOT_PATERN:
			ShootPatern(_world, _dt);
			break;
		case RAT_BALL:
			RatBallPatern(_world, _dt);
			break;
		default:
			break;
		}


		if (m_hp <= 0)
		{
			m_stateBoss = DEAD;
			m_hp = 100;
			TurnToPlayer();
			m_offSetAnim = { 0,0 };

			m_bossAnim->timeAnim = 0;
			m_animState = DEAD_ANIM;
		}
	}
	else if (m_stateBoss == DEAD)
	{
		m_bossAnim->timeAnim += _dt;



		if (m_animState == DEAD_BOUCLE)
		{
			Animation(m_spritesPandemic[BIG_RAT_SPRITE], m_bigRatAnim, 0, _dt, ANIM_SIZE_MAX_BIG_RAT);
		}

		if (GetActualyFrame(m_bossAnim) >= m_bossAnim[DEAD_ANIM].frameNb && m_animState == DEAD_ANIM)
		{
			sf::Vector2f playerPos = World2Screen(m_playerPos);

			m_bossAnim->timeAnim = 0;
			m_animState = DEAD_BOUCLE;

			m_bigRatAnim->timeAnim = 0;
			m_spritesPandemic[BIG_RAT_SPRITE].setPosition(sf::Vector2f(playerPos.x + 125, 265));

		}

		if (GetActualyFrame(m_bigRatAnim) >= 8)
		{
			m_playerEated = true;
		}

		if (m_animState == DEAD_BOUCLE && GetActualyFrame(m_bigRatAnim) >= m_bigRatAnim->frameNb)
		{
			m_hp = 0;
			Clear(_world);
		}
		else
		{
			m_hp = 100;
		}


	}
}

void BossPandemicData::Display(sf::RenderWindow& _window)
{

	for (int i = 0; i < TOT_LAYER_BACKGROUND - 1; i++)
	{

		if (i == 3)
		{
			Animation(m_spritesPandemic[MIST_SPRITE], m_mistAnimBack, 0, 0, 15360);
			BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(-1920 + MIST_X_OFFSET, 550), 0, _window);
			BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(0 + MIST_X_OFFSET, 550), 0, _window);
			BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(1920 + MIST_X_OFFSET, 550), 0, _window);
		}

		if (i == 5)
		{
			if (m_stateBoss <= SPAWN_BOSS)
			{
				_window.draw(m_spritesPandemic[SPAWN_BOSS_SPRITE]);
			}


			Animation(m_spritesPandemic[MIST_SPRITE], m_mistAnim, 0, 0, 15360);
			BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(-1920, 550), 0, _window);
			BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(0, 550), 0, _window);

			if (m_stateBoss == START_FIGHT || m_stateBoss == DEAD)
			{
				for (auto& ratShoot : m_ratShoot)
				{
					if (RadToDeg(-ratShoot.dir) + 180 <= -120)
					{
						m_spritesPandemic[RAT_SHOOT].setScale(sf::Vector2f(1, -1));
					}
					else
					{
						m_spritesPandemic[RAT_SHOOT].setScale(sf::Vector2f(1, 1));
					}

					Animation(m_spritesPandemic[RAT_SHOOT], ratShoot.anim, 0, 0);
					BlitSprite(m_spritesPandemic[RAT_SHOOT], World2Screen(ratShoot.body->GetPosition()), RadToDeg(-ratShoot.dir) + 180, _window);
				}


				if (m_spearBody != nullptr)
				{
					sf::Vector2f spearPos = World2Screen(m_spearBody->GetPosition());
					spearPos.x += m_spritesPandemic[SPEAR].getGlobalBounds().width / 2;

					BlitSprite(m_spritesPandemic[SPEAR], spearPos, 0, _window);
				}

				if (m_ratBallBody != nullptr)
				{
					sf::Vector2f ratBallPos = World2Screen(m_ratBallBody->GetPosition());
					if (m_dirRatBall == RAT_BALL_LEFT)
					{
						ratBallPos.x += m_spritesPandemic[RAT_BALL_SPRITE].getGlobalBounds().width / 2;

					}

					BlitSprite(m_spritesPandemic[RAT_BALL_SPRITE], ratBallPos, 0, _window);
				}

				//Main BossPos
				sf::Vector2f bossPos;
				if (m_statusAnimJump == END_JUMP)
				{
					//in this case m_offSetAnim give the ground position in meter
					float posGroundY = World2ScreenVec(m_offSetAnim).y;

					bossPos = World2Screen(m_body->GetPosition());
					bossPos.x += m_spriteSize["Boss Idle"].x / 2;
					bossPos.y = posGroundY - 975;

				}
				else
				{
					bossPos = World2Screen(m_body->GetPosition()) + m_offSetAnim;
					bossPos.x += m_spriteSize["Boss Idle"].x / 2;
					bossPos.y -= m_spriteSize["Boss Idle"].y / 5.f;
				}

				Animation(m_spritesPandemic[BOSS_SPRITE], m_bossAnim, m_animState, 0, m_sizeMaxAnimBoss);
				BlitSprite(m_spritesPandemic[BOSS_SPRITE], bossPos, 0, _window);
				//end main posBoss 



				for (auto& body : m_bodyPic)
				{
					BlitSprite(m_spritesPandemic[PIC_SPRITE], World2Screen(body->GetPosition()), 0, _window);
				}


				if (m_slashs[0] != nullptr)
				{
					m_spritesPandemic[SLASH].setScale(sf::Vector2f(1, 1));
					BlitSprite(m_spritesPandemic[SLASH], World2Screen(m_slashs[0]->GetPosition()), 0, _window);

				}

				if (m_slashs[1] != nullptr)
				{

					m_spritesPandemic[SLASH].setScale(sf::Vector2f(-1, 1));
					BlitSprite(m_spritesPandemic[SLASH], World2Screen(m_slashs[1]->GetPosition()), 0, _window);
				}


			}
		}

		_window.draw(m_backgroundLayer[i]);
	}


	if (m_stateBoss == OFF)
	{
		Animation(m_spritesPandemic[SKULL_SPRITE], m_skullAnim, m_statusAnimSkull, 0);
		_window.draw(m_spritesPandemic[SKULL_SPRITE]);
	}

}

void BossPandemicData::DisplayFront(sf::RenderWindow& _window)
{
	if (m_playerEated)
	{
		for (int i = 0; i < TOT_LAYER_BACKGROUND; i++)
		{
			if (i == 3)
			{
				Animation(m_spritesPandemic[MIST_SPRITE], m_mistAnimBack, 0, 0, 15360);
				BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(-1920 + MIST_X_OFFSET, 550), 0, _window);
				BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(0 + MIST_X_OFFSET, 550), 0, _window);
				BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(1920 + MIST_X_OFFSET, 550), 0, _window);
			}

			if (i == 5)
			{
				Animation(m_spritesPandemic[MIST_SPRITE], m_mistAnim, 0, 0, 15360);
				BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(-1920, 550), 0, _window);
				BlitSprite(m_spritesPandemic[MIST_SPRITE], sf::Vector2f(0, 550), 0, _window);

				//Main BossPos
				sf::Vector2f bossPos;
				bossPos = World2Screen(m_body->GetPosition()) + m_offSetAnim;
				bossPos.x += m_spriteSize["Boss Idle"].x / 2;
				bossPos.y -= m_spriteSize["Boss Idle"].y / 5.f;

				Animation(m_spritesPandemic[BOSS_SPRITE], m_bossAnim, m_animState, 0, m_sizeMaxAnimBoss);
				BlitSprite(m_spritesPandemic[BOSS_SPRITE], bossPos, 0, _window);
				//end main posBoss 

				_window.draw(m_spritesPandemic[BIG_RAT_SPRITE]);

			}

			_window.draw(m_backgroundLayer[i]);
		}
	}
	else
	{

		if (m_stateBoss == DEAD)
		{
			_window.draw(m_spritesPandemic[BIG_RAT_SPRITE]);
		}

		_window.draw(m_backgroundLayer[6]);
	}



}

void BossPandemicData::SpawnBoss(b2World* _world)
{
	sf::Vector2f sizeBoss = Screen2WorldVec(sf::Vector2f(m_spriteSize["Boss Idle"]));
	B2Dinfo bossInfo;

	sizeBoss.y += 2;
	bossInfo.type = b2_dynamicBody;
	bossInfo.shape = POLYGON;
	bossInfo.userData.fixtureName = "Enemy";
	bossInfo.userData.sizePolygone.width = sizeBoss.x;
	bossInfo.userData.sizePolygone.height = sizeBoss.y;
	bossInfo.userData.interruptGrapin = false;
	bossInfo.pos = Screen2WorldVec(m_spritesPandemic[SPAWN_BOSS_SPRITE].getPosition()) - (sizeBoss / 2.f);
	bossInfo.pos.y -= 1;
	bossInfo.points.push_back(b2Vec2(0, 0));
	bossInfo.points.push_back(b2Vec2(sizeBoss.x, 0));
	bossInfo.points.push_back(b2Vec2(sizeBoss.x, sizeBoss.y));
	bossInfo.points.push_back(b2Vec2(0, sizeBoss.y));


	bossInfo.density = 1;
	bossInfo.friction = 1;
	bossInfo.fixedRotation = true;

	m_body = CreateBodyB2D(_world, bossInfo, BOSS, BOUNDARY);
	m_body->SetGravityScale(2.5);
}

void BossPandemicData::HitGround(b2World* _world, CamData* _camData)
{
	if (m_paterns == CHARGE)
	{
		B2Dinfo slashInfo;
		sf::Vector2f sizeSlash = Screen2WorldVec(sf::Vector2f(sf::Vector2i(533, 361).x / 4, sf::Vector2i(533, 361).y)); // for some reasons i can't use m_sprite["Slash"]
		SizePolygone sizeBody = ((UserData*)m_body->GetFixtureList()[0].GetUserData().pointer)->sizePolygone;

		m_aimPlayer = HIT_GROUND;

		slashInfo.type = b2_dynamicBody;
		slashInfo.shape = POLYGON;
		slashInfo.userData.fixtureName = "Slash left";
		slashInfo.userData.interruptGrapin = false;
		slashInfo.pos = sf::Vector2f(m_body->GetPosition().x + sizeBody.width / 2, GetBodyOfMap(_world, "Ground")->GetPosition().y - sizeSlash.y);
		slashInfo.points.push_back(b2Vec2(0, 0));
		slashInfo.points.push_back(b2Vec2(sizeSlash.x, 0));
		slashInfo.points.push_back(b2Vec2(sizeSlash.x, sizeSlash.y));
		slashInfo.points.push_back(b2Vec2(0, sizeSlash.y));
		slashInfo.density = 1;
		slashInfo.friction = 1;
		slashInfo.fixedRotation = true;


		m_slashs[0] = CreateBodyB2D(_world, slashInfo);
		m_slashs[0]->SetGravityScale(0);
		m_slashs[0]->GetFixtureList()->SetSensor(true);

		slashInfo.userData.fixtureName = "Slash right";
		m_slashs[1] = CreateBodyB2D(_world, slashInfo);
		m_slashs[1]->SetGravityScale(0);
		m_slashs[1]->GetFixtureList()->SetSensor(true);

		_camData->ShakeCamera(50, 25, 0.25);
	}
}

void BossPandemicData::ChargePatern(b2World* _world, float _dt)
{
	if (m_statusAnimJump == START_JUMP)
	{
		m_bossAnim->timeAnim += _dt;
		m_offSetAnim = sf::Vector2f(m_spriteSize["Boss Idle"] - m_spriteSize["Boss Jump start"]);

		if (GetBodyOfMap(_world, "Ground")->GetPosition().y + Pixel2Meter(m_spriteSize["Boss Jump start"].y - 10) > m_body->GetPosition().y && m_body->GetLinearVelocity().y < -1)
		{
			m_statusAnimJump = END_JUMP;
			m_body->SetLinearVelocity(b2Vec2(0, m_body->GetLinearVelocity().y));

			m_bossAnim->timeAnim = 0;
			m_animState = JUMP_END_ANIM;
		}
	}

	if (m_statusAnimJump == END_JUMP)
	{
		m_bossAnim->timeAnim += _dt;
		m_offSetAnim = sf::Vector2f(0, GetBodyOfMap(_world, "Ground")->GetPosition().y);

		if (GetActualyFrame(m_bossAnim) <= 6)
		{
			m_body->SetLinearVelocity(b2Vec2(0, 0));
		}
		else
		{
			m_body->SetLinearVelocity(b2Vec2(0, -50));
		}

		if (GetActualyFrame(m_bossAnim) >= m_bossAnim[JUMP_END_ANIM].frameNb)
		{
			m_statusAnimJump = RE_SPAWN;
			m_animState = TP_ANIM;
			m_sizeMaxAnimBoss = ANIM_SIZE_MAX_PIC_AND_TP;
			m_bossAnim->timeAnim = (1.f / m_bossAnim[TP_ANIM].framerate) * m_bossAnim[TP_ANIM].frameNb;
			TurnToPlayer();
		}

		//RE_SPAWN
	}

	if (m_statusAnimJump == RE_SPAWN)
	{
		m_bossAnim->timeAnim -= _dt;
		m_offSetAnim = sf::Vector2f(m_spriteSize["Boss Idle"] - m_spriteSize["Boss Tp"]);
	}


	if (m_slashs[0] != nullptr || m_slashs[1] != nullptr)
	{
		Animation(m_spritesPandemic[SLASH], m_slashAnim, 0, _dt);
	}

	if (m_aimPlayer != HIT_GROUND)
	{

		if (GetActualyFrame(m_bossAnim) >= m_bossAnim[JUMP_START_ANIM].frameNb)
		{
			if (m_aimPlayer == START_AIM)
			{
				SizePolygone sizeBody = ((UserData*)m_body->GetFixtureList()[0].GetUserData().pointer)->sizePolygone;

				if (m_turnSpriteToPlayer.x == 1)
				{
					m_aimPlayer = LEFT_AIM;
				}
				else
				{
					m_aimPlayer = RIGHT_AIM;
				}


				if (m_aimPlayer == RIGHT_AIM)
				{
					m_body->ApplyLinearImpulseToCenter(b2Vec2(800, 1450), true);
				}
				else
				{
					m_body->ApplyLinearImpulseToCenter(b2Vec2(-800, 1450), true);
				}
			}
		}
	}
	else if (m_slashs[0] != nullptr || m_slashs[1] != nullptr)
	{
		if (m_slashs[0] != nullptr)
		{
			m_slashs[0]->SetLinearVelocity(b2Vec2(-10, 0));
		}

		if (m_slashs[1] != nullptr)
		{
			m_slashs[1]->SetLinearVelocity(b2Vec2(10, 0));
		}

	}
	else if (m_animState == TP_ANIM && GetActualyFrame(m_bossAnim) <= 0)
	{
		m_offSetAnim = { 0,0 };
		m_globalTimer = TIME_RECOVER;
		m_paterns = NO_PATERN;
		m_animState = IDLE_ANIM;
		m_bossAnim->timeAnim = 0;
		m_sizeMaxAnimBoss = ANIM_SIZE_MAX_DEFAULT;
	}
}

void BossPandemicData::PicPatern(b2World* _world, float _dt)
{
	float frameTimePic = 1.f / m_picAnim->framerate;
	int frameNbPic = m_picAnim->timeAnim / frameTimePic;

	m_bossAnim->timeAnim += _dt;

	if (GetActualyFrame(m_bossAnim) >= m_bossAnim[PIC_ANIM].frameNb)
	{
		m_bossAnim->timeAnim = 0;
		m_animState = IDLE_ANIM;
		m_sizeMaxAnimBoss = ANIM_SIZE_MAX_DEFAULT;
	}

	Animation(m_spritesPandemic[PIC_SPRITE], m_picAnim, 0, _dt);

	if (frameNbPic >= 19 && m_picStatus == SHOWING)
	{
		if (m_animState == IDLE_ANIM)
		{
			m_animState = PIC_ANIM;
			m_sizeMaxAnimBoss = ANIM_SIZE_MAX_PIC_AND_TP;
			m_bossAnim->timeAnim = 0;
		}

		if (GetActualyFrame(m_bossAnim) >= 10)
		{
			m_picStatus = DAMAGE;
			m_picAnim->frameNb = 42;
			m_picAnim->timeAnim = (1.f / m_picAnim->framerate) * 20;

			for (auto& body : m_bodyPic)
			{
				b2Filter filter = body->GetFixtureList()->GetFilterData();
				filter.maskBits = PLAYER;
				body->GetFixtureList()->SetFilterData(filter);
			}
		}
	}

	if (frameNbPic >= 39 && m_picStatus == DAMAGE)
	{
		m_picStatus = NO_DAMAGE;

		for (auto& body : m_bodyPic)
		{
			b2Filter filter = body->GetFixtureList()->GetFilterData();
			filter.maskBits = BOUNDARY;
			body->GetFixtureList()->SetFilterData(filter);
		}
	}

	if (m_picStatus == NO_DAMAGE)
	{
		m_timerPic += _dt;

		if (m_timerPic >= PIC_TIME_DISPAWN)
		{
			for (auto& body : m_bodyPic)
			{
				_world->DestroyBody(body);
				body = nullptr;
			}

			m_bodyPic.clear();

			m_globalTimer = TIME_RECOVER;
			m_paterns = NO_PATERN;
			m_picAnim->timeAnim = 0;
			m_picAnim->frameNb = 19;
		}
	}

}

void BossPandemicData::CratePic(b2World* _world)
{
	b2Vec2 picPos = GetBodyOfMap(_world, "StartBoss Wall")->GetPosition();
	sf::Vector2f sizePic = Screen2WorldVec(sf::Vector2f(m_spriteSize["Pic"]));
	float maxPosX = GetBodyOfMap(_world, "Right Wall")->GetPosition().x;

	sizePic.y += 0.125;
	sizePic.x /= 2;

	picPos.y -= sizePic.y;

	for (int i = 0; i < 1; i++)
	{
		if (picPos.x < maxPosX)
		{
			B2Dinfo picInfo;

			picInfo.type = b2_kinematicBody;
			picInfo.shape = POLYGON;
			picInfo.userData.fixtureName = "Pic";
			picInfo.userData.sizePolygone.width = sizePic.x;
			picInfo.userData.sizePolygone.height = sizePic.y;
			picInfo.userData.interruptGrapin = false;
			picInfo.pos = sf::Vector2f(picPos.x, picPos.y);
			picInfo.points.push_back(b2Vec2(0, 0));
			picInfo.points.push_back(b2Vec2(sizePic.x, 0));
			picInfo.points.push_back(b2Vec2(sizePic.x, sizePic.y));
			picInfo.points.push_back(b2Vec2(0, sizePic.y));
			picInfo.density = 1;
			picInfo.friction = 1;
			picInfo.fixedRotation = true;


			m_bodyPic.push_back(CreateBodyB2D(_world, picInfo, PIC, BOUNDARY));
			m_bodyPic[m_bodyPic.size() - 1]->GetFixtureList()->SetSensor(true);


			i = -1;
			picPos.x += sizePic.x + SPACE_INTER_PIC;
		}
	}



}

void BossPandemicData::ShootPatern(b2World* _world, float _dt)
{
	if (m_stateShootPatern == START_SHOOT)
	{
		m_bossAnim->timeAnim += _dt;
		m_offSetAnim = sf::Vector2f(m_spriteSize["Boss Idle"] - m_spriteSize["Boss Rat shoot"]);


		if (GetActualyFrame(m_bossAnim) >= m_bossAnim[RAT_SHOOT_ANIM].frameNb)
		{
			m_timerShoot += _dt;

			if (m_timerShoot > TIME_RAT_SHOOT)
			{
				B2Dinfo ratInfo;
				RatShoot ratShoot;
				SizePolygone bossSize = ((UserData*)m_body->GetFixtureList()->GetUserData().pointer)->sizePolygone;

				m_timerShoot = 0;


				ratInfo.type = b2_dynamicBody;
				ratInfo.shape = CIRCLE;
				ratInfo.userData.fixtureName = "Rat";
				ratInfo.userData.interruptGrapin = false;
				ratInfo.userData.m_ratShootVec = &m_ratShoot;
				ratInfo.pos = sf::Vector2f(m_body->GetPosition().x + bossSize.width / 2, m_body->GetPosition().y + bossSize.height / 2 + 1);
				ratInfo.radius = 0.5;
				ratInfo.density = 1;
				ratInfo.friction = 1;
				ratInfo.fixedRotation = true;


				ratShoot.body = CreateBodyB2D(_world, ratInfo);
				ratShoot.body->GetFixtureList()->SetSensor(true);
				ratShoot.anim[0] = InitAnimation(m_spriteSize["Rat Shoot"], 6, 3, 0, true);

				if (m_turnSpriteToPlayer.x == 1) // 1 correpond to left Side
				{
					ratShoot.dir = DegToRad(240) - DegToRad(20) * m_nbRatShoot;
				}
				else
				{
					ratShoot.dir = DegToRad(300) + DegToRad(20) * m_nbRatShoot;
				}


				m_ratShoot.push_back(ratShoot);
				m_ratShoot[m_ratShoot.size() - 1].body->SetLinearVelocity(b2Vec2(SPEED_RAT_SHOOT * cos(ratShoot.dir), SPEED_RAT_SHOOT * sin(ratShoot.dir)));
				m_ratShoot[m_ratShoot.size() - 1].body->SetGravityScale(0);
				if (m_nbRatShoot > 6)
				{
					m_stateShootPatern = END_SHOOT;
					m_bossAnim->timeAnim = (1.f / m_bossAnim[RAT_SHOOT_ANIM].framerate) * m_bossAnim[RAT_SHOOT_ANIM].frameNb;
				}

				m_nbRatShoot++;
			}
		}
	}

	if (m_stateShootPatern == END_SHOOT)
	{
		m_bossAnim->timeAnim -= _dt;

		if (GetActualyFrame(m_bossAnim) <= 0)
		{
			m_animState = IDLE_ANIM;
			m_bossAnim->timeAnim = 0;
			m_paterns = NO_PATERN;
			m_globalTimer = TIME_RECOVER;

			m_offSetAnim = { 0,0 };
		}
	}
}


void BossPandemicData::DestroySlash(int _slashDestroy)
{
	if (_slashDestroy == 0)
	{
		m_slashs[0] = nullptr;
	}

	if (_slashDestroy == 1)
	{
		m_slashs[1] = nullptr;
	}
}

void BossPandemicData::TurnToPlayer()
{
	SizePolygone bossSize = ((UserData*)m_body->GetFixtureList()->GetUserData().pointer)->sizePolygone;
	if (m_playerPos.x < m_body->GetPosition().x + bossSize.width / 2)
	{
		m_turnSpriteToPlayer.x = 1;
	}
	else
	{
		m_turnSpriteToPlayer.x = -1;
	}
}

void BossPandemicData::RatBallPatern(b2World* _world, float _dt)
{

	Animation(m_spritesPandemic[RAT_BALL_SPRITE], m_ratBallAnim, 0, _dt, ANIM_SIZE_MAX_RAT_BALL);

	if (m_statusRatBall == START_PHASE)
	{
		m_bossAnim->timeAnim += _dt;

		if (GetActualyFrame(m_ratBallAnim) >= 39 && m_ratBallAnim->frameNb == 43)
		{
			b2Vec2 newBossPos = m_ratBallBody->GetPosition();
			SizePolygone bossSize = ((UserData*)m_body->GetFixtureList()->GetUserData().pointer)->sizePolygone;

			newBossPos.y -= 3.3;

			if (m_dirRatBall == RAT_BALL_RIGHT)
			{
				newBossPos.x -= 5;
			}

			m_statusRatBall = END_PHASE;
			m_bossAnim->timeAnim = (1.f / m_bossAnim[TP_ANIM].framerate) * m_bossAnim[TP_ANIM].frameNb;
			m_body->SetTransform(newBossPos, 0);
		}
	}
	else
	{
		TurnToPlayer();
		m_bossAnim->timeAnim -= _dt;

		if (GetActualyFrame(m_bossAnim) <= 0)
		{
			_world->DestroyBody(m_ratBallBody);
			m_ratBallBody = nullptr;
			m_ratBallAnim->timeAnim = 0;
			m_ratBallAnim->frameNb = 19;

			m_animState = IDLE_ANIM;
			m_globalTimer = TIME_RECOVER;
			m_paterns = NO_PATERN;
			m_sizeMaxAnimBoss = ANIM_SIZE_MAX_DEFAULT;

		}
	}


}

void BossPandemicData::CreateSpear(b2World* _world)
{
	B2Dinfo spearInfo;
	sf::Vector2f sizeSpear = { Pixel2Meter(m_spritesPandemic[SPEAR].getGlobalBounds().width), Pixel2Meter(m_spritesPandemic[SPEAR].getGlobalBounds().height) };
	SizePolygone bossSize = ((UserData*)m_body->GetFixtureList()->GetUserData().pointer)->sizePolygone;

	spearInfo.type = b2_dynamicBody;
	spearInfo.shape = POLYGON;
	spearInfo.userData.fixtureName = "Spear";

	spearInfo.userData.interruptGrapin = false;
	spearInfo.pos = sf::Vector2f(m_body->GetPosition().x + bossSize.width / 2 - sizeSpear.x / 2, m_body->GetPosition().y + bossSize.height / 2.25 + sizeSpear.y);
	spearInfo.points.push_back(b2Vec2(0, 0));
	spearInfo.points.push_back(b2Vec2(sizeSpear.x, 0));
	spearInfo.points.push_back(b2Vec2(sizeSpear.x, -sizeSpear.y));
	spearInfo.points.push_back(b2Vec2(0, -sizeSpear.y));

	spearInfo.density = 1;
	spearInfo.friction = 1;
	spearInfo.fixedRotation = true;

	m_spearBody = CreateBodyB2D(_world, spearInfo);
	m_spearBody->GetFixtureList()->SetSensor(true);
	m_spearBody->SetGravityScale(0);
	m_spearBody->SetLinearVelocity(b2Vec2(-SPEED_SPEAR * m_dirRatBall, 0));

	m_spritesPandemic[SPEAR].setScale(sf::Vector2f(m_dirRatBall, 1));
}

void BossPandemicData::CreateRatBall(b2World* _world)
{
	SizePolygone bossSize = ((UserData*)m_body->GetFixtureList()->GetUserData().pointer)->sizePolygone;
	b2Vec2 leftWallPos = GetBodyOfMap(_world, "StartBoss Wall")->GetPosition();
	b2Vec2 rightWallPos = GetBodyOfMap(_world, "Right Wall")->GetPosition();
	b2Vec2 bossPos = m_body->GetPosition();

	float sizeRatBallInMeter = Pixel2Meter(m_spritesPandemic[RAT_BALL_SPRITE].getGlobalBounds().width);

	if (m_turnSpriteToPlayer.x == RAT_BALL_LEFT && leftWallPos.x < bossPos.x - sizeRatBallInMeter - bossSize.width)
	{
		m_dirRatBall = RAT_BALL_LEFT;
	}
	else if (rightWallPos.x > bossPos.x + sizeRatBallInMeter + bossSize.width)
	{
		m_dirRatBall = RAT_BALL_RIGHT;
	}
	else
	{
		m_dirRatBall = RAT_BALL_LEFT;
	}

	if (m_dirRatBall == RAT_BALL_LEFT)
	{
		B2Dinfo ratBallInfo;
		b2Vec2 groundPos = GetBodyOfMap(_world, "Ground")->GetPosition();
		sf::Vector2f sizeRatBall = { Pixel2Meter(m_spriteSize["Rat Ball"].x / 2), Pixel2Meter(m_spriteSize["Rat Ball"].y) };

		ratBallInfo.type = b2_kinematicBody;
		ratBallInfo.shape = POLYGON;
		ratBallInfo.userData.fixtureName = "Rat Ball";

		ratBallInfo.userData.interruptGrapin = false;
		ratBallInfo.pos = sf::Vector2f(leftWallPos.x, groundPos.y + sizeRatBall.y);
		ratBallInfo.points.push_back(b2Vec2(0, 0));
		ratBallInfo.points.push_back(b2Vec2(sizeRatBall.x, 0));
		ratBallInfo.points.push_back(b2Vec2(sizeRatBall.x, -sizeRatBall.y));
		ratBallInfo.points.push_back(b2Vec2(0, -sizeRatBall.y));


		ratBallInfo.density = 1;
		ratBallInfo.friction = 1;
		ratBallInfo.fixedRotation = true;

		m_ratBallBody = CreateBodyB2D(_world, ratBallInfo);
		m_ratBallBody->GetFixtureList()->SetSensor(true);

		m_spritesPandemic[RAT_BALL_SPRITE].setScale(sf::Vector2f(-1, 1)); // y

	}
	else
	{
		B2Dinfo ratBallInfo;
		b2Vec2 groundPos = GetBodyOfMap(_world, "Ground")->GetPosition();
		sf::Vector2f sizeRatBall = { Pixel2Meter(m_spriteSize["Rat Ball"].x / 2), Pixel2Meter(m_spriteSize["Rat Ball"].y) };


		ratBallInfo.type = b2_kinematicBody;
		ratBallInfo.shape = POLYGON;
		ratBallInfo.userData.fixtureName = "Rat Ball";

		ratBallInfo.userData.interruptGrapin = false;
		ratBallInfo.pos = sf::Vector2f(rightWallPos.x - sizeRatBall.x, groundPos.y + sizeRatBall.y);
		ratBallInfo.points.push_back(b2Vec2(0, 0));
		ratBallInfo.points.push_back(b2Vec2(sizeRatBall.x, 0));
		ratBallInfo.points.push_back(b2Vec2(sizeRatBall.x, -sizeRatBall.y));
		ratBallInfo.points.push_back(b2Vec2(0, -sizeRatBall.y));

		ratBallInfo.density = 1;
		ratBallInfo.friction = 1;
		ratBallInfo.fixedRotation = true;

		m_ratBallBody = CreateBodyB2D(_world, ratBallInfo);
		m_ratBallBody->GetFixtureList()->SetSensor(true);

		m_spritesPandemic[RAT_BALL_SPRITE].setScale(sf::Vector2f(1, 1)); // y
	}

	m_turnSpriteToPlayer.x = m_dirRatBall;
}

void BossPandemicData::SpearHit(b2World* _world)
{
	_world->DestroyBody(m_spearBody);
	m_spearBody = nullptr;

	m_ratBallAnim->frameNb = 43;

	m_ratBallAnim->timeAnim = (1.f / m_ratBallAnim->framerate) * 18;
}

void BossPandemicData::Clear(b2World* _world)
{
	if (m_slashs[0] != nullptr)
	{
		_world->DestroyBody(m_slashs[0]);
		m_slashs[0] = nullptr;
	}

	if (m_slashs[1] != nullptr)
	{
		_world->DestroyBody(m_slashs[1]);
		m_slashs[1] = nullptr;
	}

	for (auto& body : m_bodyPic)
	{
		_world->DestroyBody(body);
		body = nullptr;
	}
	m_bodyPic.clear();

	if (m_spearBody != nullptr)
	{
		_world->DestroyBody(m_spearBody);
		m_spearBody = nullptr;
	}

	if (m_ratBallBody != nullptr)
	{
		_world->DestroyBody(m_ratBallBody);
		m_ratBallBody = nullptr;
	}

}