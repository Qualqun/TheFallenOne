#include "BossDeath.h"

BossDeath::BossDeath(b2World* _world, sf::Vector2f _pos)
{
	m_sprite = LoadSprite("Sprites/Boss/Death/DeathSpritesheetIntro.png", false);
	//m_completeSprite = LoadSprite("Sprites/Boss/Death/SpritesheetBossComplete.png", false);
	m_idleSprite = LoadSprite("Sprites/Boss/Death/Death_Idle.png", true);
	m_idleSpriteWO = LoadSprite("Sprites/Boss/Death/Death_Idle_WO.png", false);
	//m_sprite = LoadSprite("Sprites/Boss/Death/SpritesheetBossComplete.png", false);
	//m_sprite.setOrigin({ 787 / 2.f, 644 / 2.f });
	m_sprite.setOrigin({ 0, 0 });
	m_fireSprite = LoadSprite("Sprites/Boss/Death/FireSpriteSheet.png", false);
	m_seaSprite = LoadSprite("Sprites/Boss/Death/Spear.png", false);
	m_seaSprite.setOrigin({ 164 / 2.f, 88.f });
	m_ballsSprite = LoadSprite("Sprites/Boss/Death/BouncingBall.png", true);
	m_laserSprite = LoadSprite("Sprites/Boss/Death/LaserSingle.png", true);
	m_ghostSprite = LoadSprite("Sprites/Boss/Death/ball.png", true);

	m_maxHp = 200;
	m_hp = 200;
	m_globalTimer = BOSS_GLOBAL_TIMER;
	m_currentPatern = IDLE_DEATH;
	m_phases = 0;
	m_trigerSpawn = false;

	B2Dinfo tempBody;
	tempBody.userData.fixtureName = "Enemy";
	tempBody.userData.interruptGrapin = false;
	tempBody.type = b2_dynamicBody;
	tempBody.shape = POLYGON;

	tempBody.pos = Screen2WorldVec(_pos);

	tempBody.points =
	{
		b2Vec2(-BOSS_WIDTH / 128, BOSS_HEIGHT / 128),
		b2Vec2(-BOSS_WIDTH / 128,-BOSS_HEIGHT / 128),
		b2Vec2(BOSS_WIDTH / 128,-BOSS_HEIGHT / 128),
		b2Vec2(BOSS_WIDTH / 128, BOSS_HEIGHT / 128),
	};

	tempBody.density = 1;
	tempBody.friction = 1;
	tempBody.fixedRotation = true;

	m_body = CreateBodyB2D(_world, tempBody, BOSS, BOUNDARY);
	m_body->SetGravityScale(0);

	m_cristalData = new CristalData();


	tempBody.shape = CIRCLE;
	tempBody.radius = 0.2f;
	tempBody.pos = { (SCREEN_WIDTH / 2) / 64.f, -(SCREEN_HEIGHT) / 64.f };
	tempBody.userData.fixtureName = "center";

	m_bodyCenter = CreateBodyB2D(_world, tempBody);
	m_bodyCenter->SetGravityScale(0);
	m_bodyCenter->GetFixtureList()->SetSensor(true);

	//Load Anim
	m_cometAnim[INTRO_ANIM];
	//m_bossAnim[IDLE_ANIM] = InitAnimation(sf::Vector2i{ 787, 644 }, 10, 10, 0, true);

	m_currentAnim = FRONT_IDLE_P1;

	//Ajout
	m_maxHp = m_hp;
	m_isAnimFinished = false;
	m_tempBossSprite = LoadSprite("Sprites/Boss/Death/SpritesheetBossComplete.png", true);
	m_bossIntro = LoadSprite("Sprites/Boss/Death/Indicator/SpriteSheetFire.png", false);
	for (int i = 0; i < TOTAL_ANIM_LINE; i++)
	{
		m_spawnAnim[i] = InitAnimation(sf::Vector2i{ 1920, 1080 }, 8, 8, 0, false); ;
	}

	m_bossAnim[FRONT_IDLE_P1] = InitAnimation(sf::Vector2i{ 239, 566 }, 11, 11, 0, true);
	m_bossAnim[SIDE_IDLE_P1] = InitAnimation(sf::Vector2i{ 231, 611 }, 8, 10, 0, true);
	m_bossAnim[FRONT_ATTACK_P1] = InitAnimation(sf::Vector2i{ 460, 560 }, 15, 18, 0, false);
	m_bossAnim[SIDE_ATTACK_P1] = InitAnimation(sf::Vector2i{ 364, 593 }, 13, 15, 0, false);
	m_bossAnim[FRONT_IDLE_P2] = InitAnimation(sf::Vector2i{ 787, 644 }, 8, 10, 0, true);
	m_bossAnim[SIDE_IDLE_P2] = InitAnimation(sf::Vector2i{ 553, 685  }, 7, 9, 0, true);
	m_bossAnim[FRONT_ATTACK_P2] = InitAnimation(sf::Vector2i{ 787, 644 }, 15, 18, 0, false);
	m_bossAnim[SIDE_ATTACK_P2] = InitAnimation(sf::Vector2i{ 364, 593 }, 13, 15, 0, false);
}

BossDeath::~BossDeath()
{

}

void BossDeath::Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData)
{
	if (m_phases == 0)
	{

		if (m_trigerSpawn == false)
		{
			b2Vec2 skullPos = GetBodyFromWorld("Skull", _world)->GetPosition();
			if (skullPos.x == 500)
			{
				m_trigerSpawn = true;
				m_timeToSpawn = 8.0f;
			}
		}
		else
		{
			m_timerSpawn += _dt;
		}
		if (!m_isAnimFinished && m_trigerSpawn)
		{
			Animation(m_sprite, m_spawnAnim, m_StartRow, _dt);
		}
		else
		{
			if (m_isAnimFinished)
			{
				Animation(m_sprite, m_bossAnim, m_currentAnim, _dt);
			}
		}
		if (m_StartRow >= 5)
		{
			_particleData->CreateBossScreamParticle(sf::Vector2f{1500, 400}, 0.5, 0.3);
			if (!_camData->IsShaking())
			{
				_camData->ShakeCamera(10, 20, 0.5);
			}
		}
		if (m_spawnAnim->timeAnim >= 1.f && m_StartRow <= 6)
		{
			m_spawnAnim->timeAnim = 0;
			m_StartRow++;

		}
		else if (m_StartRow > 6)
		{

			m_body->SetTransform({ 1400.f / 64.f , -600 / 64.f }, 0);
			Teleport(Screen2World({ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 10.f * 4 }), false);
			m_isAnimFinished = true;
			m_trigerSpawn = true;
			m_sprite = m_tempBossSprite;
			m_currentPatern = IDLE_DEATH;
			m_currentAnim = FRONT_IDLE_P1;
			m_sprite.setOrigin({ 100, 644 / 2.f });
			Animation(m_sprite, m_bossAnim, m_currentAnim, _dt);
			m_phases = 1;
			m_gamePhase = REAPWAR_FIRST_PHASE;
			m_globalTimer = 0;
		}
	}
	if (m_phases == 1 || m_phases == 2)
	{
		if (m_globalTimer <= 0 && m_currentPatern == IDLE_DEATH && m_isAnimFinished) //Get a new pattern
		{
			switch (m_phases)
			{
			case 1:
				m_currentPatern = rand() % PHASE2;
				m_atk = (m_atk + 1) % PHASE2;

				break;
			case 2:
				m_currentPatern = rand() % (IDLE_DEATH - PHASE2) + PHASE2;

				m_atk++;
				if (m_atk == IDLE_DEATH || m_atk < COMET)
				{
					m_atk = COMET;
				}
				break;
			default:
				break;
			}

			//Override of the random patern
			//m_currentPatern = SOUL_PROJECTION;
			//m_atk = m_currentPatern;

			//Skip patterne

			if (m_atk == SHOCK_WAVE)
			{
				m_atk = SEA_SOULS;
			}
			if (m_atk == SOUL_PROJECTION)
			{
				m_atk = SHRINKING_CIRCLE;
			}
			if (m_atk == COMET)
			{
				m_atk = BOUNCING_BALLS;
			}
			m_currentPatern = m_atk;
			m_atkInit = false;

			if (m_phases == 1)
			{
				if (m_isInSide)
				{
					m_currentAnim = SIDE_ATTACK_P1;
				}
				else
				{
					m_currentAnim = FRONT_ATTACK_P1;
				}
			}
			if (m_phases == 2)
			{
				if (m_isInSide)
				{
					m_currentAnim = SIDE_ATTACK_P2;
				}
				else
				{
					m_currentAnim = FRONT_ATTACK_P2;
				}
			}
			_indicatorData->CreateIndicator(AME, World2Screen(m_body->GetPosition()));

		}
		else if (m_currentPatern != IDLE_DEATH) //Update the pattern
		{
			if (m_atkInit == false && _indicatorData->FinishedPlaying() == true && m_finishedTp == true)
			{
				switch (m_currentPatern)
				{
				case SEA_SOULS:
					SeaSoulsInit(_world);
					break;
				case SOUL_PROJECTION:
					SoulProjectionInit(_world);
					break;
				case SHRINKING_CIRCLE:
					ShrinkingCircleInit(_world);
					break;
				case FIRE_GROUND:
					FireGroundInit(_world);
					break;
				case SHOCK_WAVE:
					ShockWaveInit(_world);
					break;
				case COMET:
					CometInit(_world);
					break;
				case BOUNCING_BALLS:
					BouncingBallsInit(_world);
					break;
				case DAMAGE_ZONE:
					DamageZoneInit(_world);
					break;
				case LASER_THEN_DASH:
					LaserThenDashInit(_world);
					break;
				case BLAST_CENTER:
					BlastCenterInit(_world);
					break;
				default:
					break;
				}
				m_atkInit = true;
			}

			if (m_atkInit == true)
			{
				switch (m_currentPatern)
				{
				case SEA_SOULS:
					SeaSoulsPatern(_world, _dt);
					break;
				case SOUL_PROJECTION:
					SoulProjectionPatern(_world, _dt);
					break;
				case SHRINKING_CIRCLE:
					ShrinkingCirclePatern();
					break;
				case FIRE_GROUND:
					FireGroundPatern(_world, _dt);
					break;
				case SHOCK_WAVE:
					ShockWavePatern();
					break;
				case COMET:
					CometPatern();
					break;
				case BOUNCING_BALLS:
					BouncingBallsPatern(_world, _dt);
					break;
				case DAMAGE_ZONE:
					DamageZonePatern(_world, _dt);
					break;
				case LASER_THEN_DASH:
					LaserThenDashPatern(_world);
					break;
				case BLAST_CENTER:
					BlastCenterPatern(_world, _dt, *_camData , *_particleData);
					break;
				default:
					break;
				}
			}
		}
		else //Decreasse the time needed before a new
		{
			m_globalTimer -= _dt;
		}

		if (m_currentPatern == IDLE_DEATH || m_currentPatern == SEA_SOULS || m_currentPatern == FIRE_GROUND)
		{
			MovementBoss(_world, _dt);
		}
		else
		{
			if (m_currentPatern != LASER_THEN_DASH)
			{
				m_body->SetLinearVelocity({ 0,0 });
			}
		}

		TpUpdate(_dt);

		if (m_ExploCometBody != nullptr)
		{
			m_timerCometExplo -= _dt;
			if (m_timerCometExplo <= 0.f)
			{
				b2Body* ExploBody = GetBodyFromWorld("ExploComet", _world);
				if (ExploBody != nullptr)
				{
					m_timerCometExplo = 1.f;
					ExploBody->DestroyFixture(ExploBody->GetFixtureList());
					_world->DestroyBody(ExploBody);
					ExploBody = nullptr;
				}
			}
		}

		if (m_cometBody != nullptr)
		{
			Animation(m_cometSprite, m_cometAnim, FALLING, _dt);
		}
		m_sprite.setPosition(World2Screen(m_body->GetPosition()));
		m_sprite.setRotation(-RadToDeg(m_body->GetAngle()));

		Animation(m_sprite, m_bossAnim, m_currentAnim, _dt);
	}
	if (m_phases == 3)
	{
		if (m_didShake == false)
		{
			_camData->ShakeCamera(5, 20, 3.f);
			m_didShake = true;
		}
	}

	//Change phase boss
	if (m_hp <= m_maxHp / 2.f)
	{
		m_phases = 2;
		m_gamePhase = REAPWAR_SECOND_PHASE;
	}
	if (m_hp <= 0)
	{
		m_phases = 3;
	}
}

void BossDeath::Display(sf::RenderWindow& _window)
{
	//Draw fire sprite
	for (int i = 0; i < m_fire.size(); i++)
	{
		if (m_fireBodyDestroyed[i] == false)
		{
			m_fireSprite.setTextureRect(m_fireIntRect[i]);
			m_fireSprite.setPosition(World2Screen(m_fire[i]->GetPosition()) - sf::Vector2f{ (float)m_fireIntRect[i].width / 2, (float)m_fireIntRect[i].height });

			_window.draw(m_fireSprite);
		}
		if (m_projection != nullptr)
		{
			BlitSprite(m_idleSprite, { m_projection->GetPosition().x * ZOOM  , -m_projection->GetPosition().y * ZOOM }, 0, _window);
		}
	}
	//Draw laser
	if (m_laser != nullptr)
	{
		m_laserSprite.setPosition((World2Screen(m_laser->GetPosition()) + sf::Vector2f{0,25}));
		_window.draw(m_laserSprite);
	}

	//Draw soul proj
	if (m_projection != nullptr)
	{
		m_idleSprite.setPosition(World2Screen(m_projection->GetPosition()));
		_window.draw(m_idleSprite);
		m_idleSprite.setPosition({ 99999,99999 });
	}

	//Draw shrinking circle
	for (int i = 0; i < m_ghostTab.size(); i++)
	{
		if (m_ghostTab[i] != NULL && m_ghostTab[i] != nullptr)
		{
			m_ghostSprite.setScale({ 0.5f , 0.5f });
			BlitSprite(m_ghostSprite, { m_ghostTab[i]->GetPosition().x * 64.f, -m_ghostTab[i]->GetPosition().y * 64.f }, 0, _window);
		}
	}

	//Draw sea
	for (int i = 0; i < m_seaSoul.size(); i++)
	{
		m_seaSprite.setPosition(World2Screen(m_seaSoul[i]->GetPosition()));

		_window.draw(m_seaSprite);
	}

	//Draw balls
	for (int i = 0; i < m_ballsBody.size(); i++)
	{
		m_ballsSprite.setPosition(World2Screen(m_ballsBody[i]->GetPosition()));
		m_ballsSprite.setRotation(RadToDeg(m_ballsBody[i]->GetAngle()));

		_window.draw(m_ballsSprite);
	}

	m_cristalData->Draw(_window);
	if (m_isAnimFinished)
	{
		sf::Vector2f m_spriteCoor; /*= { m_sprite.getTextureRect().width / 2.f  , m_sprite.getTextureRect().height / 2.f};*/
		m_spriteCoor = { m_body->GetPosition().x * ZOOM - m_sprite.getTextureRect().width / 3, -m_body->GetPosition().y * ZOOM };
		m_sprite.setPosition((m_spriteCoor));
	}
	if (m_trigerSpawn)
	{
		_window.draw(m_sprite);
	}
}

void BossDeath::ResetAtkState()
{
	m_currentPatern = IDLE_DEATH;
	if (m_phases == 1)
	{
		if (m_isInSide)
		{
			m_currentAnim = SIDE_IDLE_P1;
		}
		else
		{
			m_currentAnim = FRONT_IDLE_P1;
		}
	}
	if (m_phases == 2)
	{
		if (m_isInSide)
		{
			m_currentAnim = SIDE_IDLE_P2;
		}
		else
		{
			m_currentAnim = FRONT_IDLE_P2;
		}
	}
	m_globalTimer = BOSS_GLOBAL_TIMER;
	m_bossAnim->timeAnim = 0;
}

#pragma region Patern
//Patern phase 1
void BossDeath::SeaSoulsPatern(b2World* _world, float _dt)
{
	if (m_seaOffset.y < Screen2World({ 0,SCREEN_HEIGHT / 4 * 3 }).y && m_seaTimer[0] <= SEA_SOUL_DURATION)
	{
		m_seaOffset.y += 2 * _dt;
	}
	else
	{
		if (m_seaTimer[0] > SEA_SOUL_DURATION)
		{
			m_seaOffset.y -= 2 * _dt;
			if (m_seaOffset.y < Screen2World({ 0,SCREEN_HEIGHT }).y - 3)
			{
				for (int i = 0; i < m_seaSoul.size(); i++)
				{
					_world->DestroyBody(m_seaSoul[i]);
				}
				m_seaSoul.clear();
				m_seaTimer.clear();
				ResetAtkState();
			}
		}
	}

	for (int i = 0; i < m_seaSoul.size(); i++)
	{
		m_seaTimer[i] += _dt;
		m_seaSoul[i]->SetTransform({ m_seaSoul[i]->GetPosition().x,-sinf(m_seaTimer[i] / 2) + m_seaOffset.y }, 0);
	}
}

void BossDeath::SoulProjectionPatern(b2World* _world, float _dt)
{
	b2Vec2 velocity = m_projection->GetLinearVelocity();
	//std::cout << "x: " << velocity.x << "\ty: " << velocity.y << std::endl;
	m_projection->ApplyLinearImpulseToCenter({ -velocity.x * SOUL_PROJ_BASE_SPEED * _dt,-velocity.y * SOUL_PROJ_BASE_SPEED * _dt }, true);
	if (velocity.x <= SOUL_PROJ_MIN_SPEED &&
		velocity.x >= -SOUL_PROJ_MIN_SPEED &&
		velocity.y <= SOUL_PROJ_MIN_SPEED &&
		velocity.y >= -SOUL_PROJ_MIN_SPEED)
	{
		b2Vec2 bossPos = m_body->GetPosition();
		b2Vec2 soulPos = m_projection->GetPosition();
		b2Vec2 dir = { bossPos.x - soulPos.x, bossPos.y - soulPos.y };

		if (m_soulGoBack == true)
		{
			_world->DestroyBody(m_projection);
			ResetAtkState();
			m_body->SetTransform(m_posMemory, 0);
		}

		if (m_soulGoBack == false)
		{
			m_projection->SetLinearVelocity({ dir.x / abs(dir.x) * SOUL_PROJ_BASE_SPEED, 0 });
			m_soulGoBack = true;
		}
	}
}

void BossDeath::ShrinkingCirclePatern()
{

}

void BossDeath::FireGroundPatern(b2World* _world, float _dt)
{
	for (int i = 0; i < m_fire.size(); i++)
	{
		if (m_fireLifetime[i] <= FIRE_LIFESPAN)
		{
			float heightFire = sinf(m_fireLifetime[i] * M_PI / FIRE_LIFESPAN) * FIRE_HEIGHT_B2D;
			b2Vec2 groundPos = GetBodyOfMap(_world, "Ground")->GetPosition();

			m_fireInfo.pos = sf::Vector2f{ m_fire[i]->GetPosition().x,m_fire[i]->GetPosition().y };
			m_fireInfo.points =
			{
				b2Vec2(-FIRE_WIDTH_B2D / 2, heightFire),
				b2Vec2(-FIRE_WIDTH_B2D / 2, 0),
				b2Vec2(FIRE_WIDTH_B2D / 2, 0),
				b2Vec2(FIRE_WIDTH_B2D / 2, heightFire)
			};
			m_fireLifetime[i] += _dt;

			_world->DestroyBody(m_fire[i]);
			CreateBodyB2D(_world, m_fireInfo);
			m_fire[i]->GetFixtureList()->SetSensor(true);

			m_fireAnim[0].timeAnim = m_fireLifetime[i];
			m_fireIntRect[i] = GetAnimIntRect(m_fireAnim, 0);
		}
		else if (m_fireBodyDestroyed[i] == false)
		{
			//Destroy old body and flag them as such
			_world->DestroyBody(m_fire[i]);
			m_fireBodyDestroyed[i] = true;
		}
		if (m_fireLifetime[m_fire.size() - 1] >= FIRE_LIFESPAN / 2)
		{
			//float heightFire = sinf(m_fireLifetime[i] * M_PI / FIRE_LIFESPAN) * FIRE_HEIGHT_B2D;
			b2Vec2 groundPos = GetBodyOfMap(_world, "Ground")->GetPosition();

			m_fireInfo.pos = sf::Vector2f{ m_fire[m_fire.size() - 2]->GetPosition().x + FIRE_WIDTH_B2D,m_fire[m_fire.size() - 2]->GetPosition().y };
			m_fireInfo.points =
			{
				b2Vec2(-FIRE_WIDTH_B2D / 2, 0),
				b2Vec2(-FIRE_WIDTH_B2D / 2, 0),
				b2Vec2(FIRE_WIDTH_B2D / 2, 0),
				b2Vec2(FIRE_WIDTH_B2D / 2, 0)
			};

			m_fire.push_back(CreateBodyB2D(_world, m_fireInfo));
			m_fireLifetime.push_back(0);
			m_fire[m_fire.size() - 1]->GetFixtureList()->SetSensor(true);
			m_fireIntRect.push_back(GetAnimIntRect(m_fireAnim, 0));
			m_fireBodyDestroyed.push_back(false);

			m_fireInfo.pos = sf::Vector2f{ m_fire[m_fire.size() - 2]->GetPosition().x - FIRE_WIDTH_B2D,m_fire[m_fire.size() - 2]->GetPosition().y };
			m_fire.push_back(CreateBodyB2D(_world, m_fireInfo));
			m_fireLifetime.push_back(0);
			m_fire[m_fire.size() - 1]->GetFixtureList()->SetSensor(true);
			m_fireIntRect.push_back(GetAnimIntRect(m_fireAnim, 0));
			m_fireBodyDestroyed.push_back(false);
		}
		if ((m_fire[m_fire.size() - 2]->GetPosition().x > World2Screen({ SCREEN_WIDTH,0 }).x ||
			m_fire[m_fire.size() - 1]->GetPosition().x < World2Screen({ 0,0 }).x) &&
			(m_fireLifetime[m_fire.size() - 2] >= FIRE_LIFESPAN / 2 || m_fireLifetime[m_fire.size() - 1] >= FIRE_LIFESPAN / 2))
		{

			for (int i = 0; i < m_fire.size(); i++)
			{
				if (m_fireBodyDestroyed[i] == false)
				{
					_world->DestroyBody(m_fire[i]);
				}
			}
			m_fire.clear();
			m_fireIntRect.clear();
			m_fireLifetime.clear();
			m_fireBodyDestroyed.clear();

			ResetAtkState();
		}
	}
}

void BossDeath::ShockWavePatern()
{

}

//Patern phase 2
void BossDeath::CometPatern()
{

}

void BossDeath::BouncingBallsPatern(b2World* _world, float _dt)
{
	m_ballsPaternTimer -= _dt;
	if (m_ballsBody.size() < BALLS_MAX_NB)
	{
		m_ballsCooldown -= _dt;
		if (m_ballsCooldown <= 0)
		{
			b2Vec2 bossPos = m_body->GetPosition();
			b2Vec2 playerPos = GetBodyFromWorld("FootPlayer", _world)->GetPosition();
			sf::Vector2f dir = { playerPos.x - bossPos.x, playerPos.y - bossPos.y };

			m_ballsInfo.userData.fixtureName = "Balls" + std::to_string(m_ballsBody.size());
			m_ballsBody.push_back(CreateBodyB2D(_world, m_ballsInfo));
			m_ballsBody[m_ballsBody.size() - 1]->GetFixtureList()->SetSensor(true);
			m_ballsBody[m_ballsBody.size() - 1]->SetGravityScale(0);
			m_ballsBody[m_ballsBody.size() - 1]->SetLinearVelocity(b2Vec2(VecDirToVecUnit(dir).x * BALLS_SPEED, VecDirToVecUnit(dir).y * BALLS_SPEED));
			m_ballsBody[m_ballsBody.size() - 1]->SetAngularVelocity(BALLS_ROTATION_SPEED);

			m_ballsCooldown = BALLS_COOLDOWN;
		}
	}
	else
	{
		bool isOutside = true;
		for (int i = 0; i < m_ballsBody.size(); i++)
		{
			sf::Vector2f ballPos = World2Screen(m_ballsBody[i]->GetPosition());
			sf::FloatRect ballHitbox = { ballPos.x, ballPos.y, BALLS_RADIUS * ZOOM,BALLS_RADIUS * ZOOM };

			if (ballHitbox.left + ballHitbox.width / 2 <= 0 ||
				ballHitbox.left - ballHitbox.width / 2 >= SCREEN_WIDTH ||
				ballHitbox.top + ballHitbox.height / 2 <= 0 ||
				ballHitbox.top - ballHitbox.height / 2 >= SCREEN_HEIGHT)
			{

			}
			else
			{
				isOutside = false;
			}
		}
		//if (m_cristalSpawned == false && m_ballsPaternTimer <= BALLS_PATERN_TIME / 2.f)
		if (m_cristalSpawned == false && m_ballsPaternTimer <= BALLS_PATERN_TIME - (BALLS_MAX_NB * BALLS_COOLDOWN))
		{
			m_cristalData->CreateCristal(_world, Screen2World({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 * 3 }));
			m_cristalSpawned = true;
		}
		if (isOutside)
		{
			for (int i = 0; i < m_ballsBody.size(); i++)
			{
				_world->DestroyBody(m_ballsBody[i]);
			}
			m_ballsBody.clear();
			m_ballsCooldown = BALLS_COOLDOWN;
			m_cristalSpawned = false;
			ResetAtkState();
			m_cristalData->DestroyAllElement(_world);
		}
		if (m_phases == 1)
		{
			if (m_isInSide)
			{
				m_currentAnim = SIDE_IDLE_P1;
			}
			else
			{
				m_currentAnim = FRONT_IDLE_P1;
			}
		}
		if (m_phases == 2)
		{
			if (m_isInSide)
			{
				m_currentAnim = SIDE_IDLE_P2;
			}
			else
			{
				m_currentAnim = FRONT_IDLE_P2;
			}
		}
	}
}

void BossDeath::DamageZonePatern(b2World* _world, float _dt)
{
	sf::Vector2f dmgPos = World2Screen(m_damageZone->GetPosition());
	if (dmgPos.x <= SCREEN_WIDTH / 4)
	{
		m_damageZone->SetLinearVelocity({ 0,0 });
		m_damageZoneArrived = true;
	}
	if (m_damageZoneArrived == true)
	{
		m_damageZoneTimer -= _dt;
	}
	if (m_damageZoneTimer <= 0)
	{
		m_damageZone->SetLinearVelocity({ 15,0 });
	}
	if (m_damageZoneTimer <= 0 && dmgPos.x > SCREEN_WIDTH)
	{
		_world->DestroyBody(m_damageZone);
		ResetAtkState();
		m_cristalData->DestroyAllElement(_world);
	}
}

void BossDeath::LaserThenDashPatern(b2World* _world)
{
	m_laser->SetTransform({ m_body->GetPosition().x,m_body->GetPosition().y - BOSS_HEIGHT / 2 / (float)ZOOM + 1 }, 0);

	if (m_body->GetLinearVelocity().x < 1 && m_body->GetLinearVelocity().x > -1)
	{
		_world->DestroyBody(m_laser);
		m_laser = nullptr;
		ResetAtkState();
		m_cristalData->DestroyAllElement(_world);
	}
}

void BossDeath::BlastCenterPatern(b2World* _world, float _dt, CamData& _camData, ParticleData& _particleData)
{
	m_explosionTimer -= _dt;
	m_blastTimer -= _dt;

	if (m_exploded == true && m_blastTimer <= 0)
	{
		m_body->SetTransform(m_posMemory, 0);

		_world->DestroyBody(m_explosion);
		ResetAtkState();

		//Cleanup cristal and safezone
		m_cristalData->DestroyAllElement(_world);

	}
	if (m_explosionTimer <= 0 && m_exploded == false)
	{
		m_exploded = true;
		m_explosion->GetFixtureList()->GetShape()->m_radius = EXPLOSION_RADIUS;
		m_explosion->SetAwake(true);
		m_blastTimer = BLAST_TIME;
		_camData.ShakeCamera(10, 30, BLAST_TIME);
	}
	if (m_blastTimer > 0 && m_exploded == true)
	{
		_particleData.CreateBossScreamParticle(World2Screen(m_explosion->GetPosition()), 0.75f, 0.1f);
	}
}
#pragma endregion

#pragma region Init patern
//Init phase 1
void BossDeath::SeaSoulsInit(b2World* _world) // Swann
{
	B2Dinfo soulBody;

	soulBody.type = b2_dynamicBody;
	soulBody.userData.fixtureName = "Projectile";
	soulBody.userData.interruptGrapin = false;
	soulBody.fixedRotation = true;
	soulBody.density = 1;
	soulBody.friction = 1;
	soulBody.radius = 0.5f;
	soulBody.shape = CIRCLE;
	for (int i = 0; i < 31; i++)
	{
		m_seaOffset = { 0,Screen2World({ 0,SCREEN_HEIGHT }).y - 3 };
		soulBody.pos = { 1.f * i, m_seaOffset.y };
		m_seaSoul.push_back(CreateBodyB2D(_world, soulBody));
		m_seaSoul[i]->GetFixtureList()->SetSensor(true);
		m_seaSoul[i]->SetGravityScale(0);
		m_seaTimer.push_back(i);
	}

	m_doMove = true;
}

void BossDeath::SoulProjectionInit(b2World* _world) // Swann
{
	m_posMemory = m_body->GetPosition();
	m_body->SetTransform(Screen2World({ SCREEN_WIDTH / 2, SCREEN_HEIGHT - BOSS_HEIGHT / 2 + 64 }), 0);
	m_body->SetAwake(true);

	b2Vec2 bossPos = m_body->GetPosition();
	b2Vec2 playerPos = GetBodyFromWorld("FootPlayer", _world)->GetPosition();
	b2Vec2 dir = { playerPos.x - bossPos.x, playerPos.y - bossPos.y };

	B2Dinfo tempBody;
	tempBody.userData.fixtureName = "Projection";
	tempBody.userData.interruptGrapin = false;
	tempBody.type = b2_dynamicBody;
	tempBody.shape = POLYGON;

	tempBody.pos = sf::Vector2f{ bossPos.x,Screen2World({0,SCREEN_HEIGHT - BOSS_HEIGHT / 2}).y + 1 };
	tempBody.points =
	{
		b2Vec2(-BOSS_WIDTH / 128, BOSS_HEIGHT / 128),
		b2Vec2(-BOSS_WIDTH / 128,-BOSS_HEIGHT / 128),
		b2Vec2(BOSS_WIDTH / 128,-BOSS_HEIGHT / 128),
		b2Vec2(BOSS_WIDTH / 128, BOSS_HEIGHT / 128),
	};

	tempBody.density = 1;
	tempBody.friction = 1;
	tempBody.fixedRotation = true;

	m_projection = CreateBodyB2D(_world, tempBody);
	m_projection->GetFixtureList()->SetSensor(true);
	m_projection->SetLinearVelocity({ dir.x / abs(dir.x) * SOUL_PROJ_BASE_SPEED, 0 });
	m_projection->SetGravityScale(0);

	m_soulGoBack = false;
	m_soulTimer = SOUL_PROJ_COOLDOWN;

	m_doMove = false;
}

void BossDeath::ShrinkingCircleInit(b2World* _world) // Leo
{
	m_cumulProj = 0;
	m_ghostTab.clear();
	b2Body* bodyPlayer = GetBodyFromWorld((std::string)"FootPlayer", _world);
	sf::Vector2f bodyPos = { SCREEN_WIDTH / 2 , SCREEN_HEIGHT };

	//Chose an angle 
	float startAngleGap = rand() % (180 - GAP_INTERVAL) + 180;

	sf::Vector2f tempPos;

	for (int i = 0; i < NB_GHOST_CIRCLEPATERN; i++)
	{
		float angle = (360.f / NB_GHOST_CIRCLEPATERN) * i;

		if (angle > startAngleGap && angle < startAngleGap + GAP_INTERVAL)
		{

		}
		else
		{
			angle = DegToRad(angle);
			tempPos.x = cosf(angle) * DISTANCE_CIRCLESHRIEKING + bodyPos.x;
			tempPos.y = sinf(angle) * DISTANCE_CIRCLESHRIEKING + bodyPos.y;

			b2Body* projectileBody = CreateProjectile(_world, 0.5f, 0.3, 70, tempPos, "tempdmg" + std::to_string(m_cumulProj));
			sf::Vector2f vecGhostPlayer;
			sf::Vector2f vecUnitGhostPlayer;

			float distGhostPlayer = sqrtf(((bodyPos.x - tempPos.x) * (bodyPos.x - tempPos.x)) + ((bodyPos.y - tempPos.y) * (bodyPos.y - tempPos.y)));
			vecGhostPlayer = { bodyPos.x - tempPos.x , bodyPos.y - tempPos.y };

			//vecteur unitaire entre le projectile et le player

			vecUnitGhostPlayer.x = (vecGhostPlayer.x / distGhostPlayer);
			vecUnitGhostPlayer.y = (vecGhostPlayer.y / distGhostPlayer);

			projectileBody->SetLinearVelocity(b2Vec2{ vecUnitGhostPlayer.x * CIRCLE_SHRIKING_SPEED ,-vecUnitGhostPlayer.y * CIRCLE_SHRIKING_SPEED });
			projectileBody->SetGravityScale(0);
			projectileBody->GetFixtureList()->SetSensor(true);

			m_ghostTab.push_back(projectileBody);
			m_cumulProj++;
		}
	}
	m_globalTimer = BOSS_GLOBAL_TIMER;
	m_currentPatern = IDLE_DEATH;
	ResetAtkState();
	m_doMove = false;
}

void BossDeath::FireGroundInit(b2World* _world) // Swann
{
	b2Vec2 groundPos = GetBodyOfMap(_world, "Ground")->GetPosition();
	b2Body* groundBody = GetBodyOfMap(_world, "Ground");

	groundBody->GetTransform().p.x / 2;


	m_fireInfo.density = 1;
	m_fireInfo.fixedRotation = true;
	m_fireInfo.friction = 0;
	m_fireInfo.points =
	{
		b2Vec2(-FIRE_WIDTH_B2D / 2, 0),
		b2Vec2(-FIRE_WIDTH_B2D / 2, 0),
		b2Vec2(FIRE_WIDTH_B2D / 2, 0),
		b2Vec2(FIRE_WIDTH_B2D / 2, 0)
	};
	m_fireInfo.shape = POLYGON;
	m_fireInfo.type = b2_kinematicBody;
	m_fireInfo.pos = sf::Vector2f{ groundBody->GetTransform().p.x / 2, groundPos.y + 3.5f };
	m_fireInfo.userData.fixtureName = "Fire";
	m_fireInfo.userData.interruptGrapin = false;


	m_fireAnim[0].frameNb = 18;
	m_fireAnim[0].framerate = m_fireAnim[0].frameNb / FIRE_LIFESPAN;
	m_fireAnim[0].isLooping = false;
	m_fireAnim[0].offset = 0;
	m_fireAnim[0].texRect = { FIRE_WIDTH,FIRE_HEIGHT };
	m_fireAnim[0].timeAnim = 0;

	m_fire.push_back(CreateBodyB2D(_world, m_fireInfo));
	m_fireLifetime.push_back(0);
	m_fire[m_fire.size() - 1]->GetFixtureList()->SetSensor(true);
	m_fireIntRect.push_back(GetAnimIntRect(m_fireAnim, 0));
	m_fireBodyDestroyed.push_back(false);

	m_fireInfo.pos = sf::Vector2f{ groundBody->GetTransform().p.x / 2, groundPos.y + 3.5f } + Screen2WorldVec({ SCREEN_WIDTH,0 });
	m_fire.push_back(CreateBodyB2D(_world, m_fireInfo));
	m_fireLifetime.push_back(0);
	m_fire[m_fire.size() - 1]->GetFixtureList()->SetSensor(true);
	m_fireIntRect.push_back(GetAnimIntRect(m_fireAnim, 0));
	m_fireBodyDestroyed.push_back(false);

	m_doMove = true;
}

void BossDeath::ShockWaveInit(b2World* _world) //Leo
{

}

//Init phase 2
void BossDeath::CometInit(b2World* _world) //Leo
{
	m_doMove = true;
}

void BossDeath::BouncingBallsInit(b2World* _world)
{
	b2Vec2 bossPos = m_body->GetPosition();
	b2Vec2 playerPos = GetBodyFromWorld("FootPlayer", _world)->GetPosition();
	sf::Vector2f dir = { playerPos.x - bossPos.x, playerPos.y - bossPos.y };

	m_doMove = false;

	m_cristalSpawned = false;

	m_ballsPaternTimer = BALLS_PATERN_TIME;

	m_ballsInfo.density = 1;
	m_ballsInfo.fixedRotation = false;
	m_ballsInfo.friction = 1;
	m_ballsInfo.pos = sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y);
	m_ballsInfo.radius = BALLS_RADIUS;
	m_ballsInfo.shape = CIRCLE;
	m_ballsInfo.type = b2_dynamicBody;
	m_ballsInfo.userData.fixtureName = "Balls" + std::to_string(m_ballsBody.size());

	m_ballsInfo.userData.interruptGrapin = false;

	m_ballsCooldown = BALLS_COOLDOWN;

	m_ballsBody.push_back(CreateBodyB2D(_world, m_ballsInfo));
	m_ballsBody[m_ballsBody.size() - 1]->GetFixtureList()->SetSensor(true);
	m_ballsBody[m_ballsBody.size() - 1]->SetGravityScale(0);
	m_ballsBody[m_ballsBody.size() - 1]->SetLinearVelocity(b2Vec2(VecDirToVecUnit(dir).x * BALLS_SPEED, VecDirToVecUnit(dir).y * BALLS_SPEED));
	m_ballsBody[m_ballsBody.size() - 1]->SetAngularVelocity(BALLS_ROTATION_SPEED);
}

void BossDeath::DamageZoneInit(b2World* _world)
{
	B2Dinfo m_damageZoneInfo;
	m_damageZoneInfo.density = 1;
	m_damageZoneInfo.fixedRotation = true;
	m_damageZoneInfo.friction = 1;
	m_damageZoneInfo.points =
	{
		b2Vec2(Screen2World({0,SCREEN_HEIGHT})),
		b2Vec2(Screen2World({0,0})),
		b2Vec2(Screen2World({SCREEN_WIDTH,0})),
		b2Vec2(Screen2World({SCREEN_WIDTH,SCREEN_HEIGHT}))
	};
	m_damageZoneInfo.shape = POLYGON;
	m_damageZoneInfo.pos = Screen2WorldVec({ SCREEN_WIDTH * 2,0 });
	m_damageZoneInfo.type = b2_dynamicBody;
	m_damageZoneInfo.userData.fixtureName = "Damage zone";
	m_damageZoneInfo.userData.interruptGrapin = false;
	m_damageZone = CreateBodyB2D(_world, m_damageZoneInfo);
	m_damageZone->SetLinearVelocity({ -5,0 });
	m_damageZone->SetGravityScale(0);
	m_damageZone->GetFixtureList()->SetSensor(true);

	//Chose a random cristal path
	switch (rand() % 3)
	{
	case 0:
		m_cristalData->CreateCristal(_world, Screen2World({ 445,372 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 637,372 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 832,372 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1030,372 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1222,372 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1357,479 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1506,600 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1671,710 }));
		break;
	case 1:
		m_cristalData->CreateCristal(_world, Screen2World({ 316,196 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 516,196 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 704,196 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 848,341 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 985,481 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1148,656 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1280,860 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1472,860 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1664,803 }));
		break;
	case 2:
		m_cristalData->CreateCristal(_world, Screen2World({ 316,760 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 512,568 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 704,376 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 800,76 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 896,376 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1088,568 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1280,760 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1472,760 }));
		m_cristalData->CreateCristal(_world, Screen2World({ 1664,703 }));
		break;
	default:
		break;
	}

	m_damageZoneTimer = DAMAGE_ZONE_TIMER;
	m_damageZoneArrived = false;

	m_posMemory = m_body->GetPosition();
	Teleport(Screen2World({ SCREEN_WIDTH - 200,SCREEN_HEIGHT / 2 + 100 }), false);
}

void BossDeath::LaserThenDashInit(b2World* _world) //Swann
{
	m_posMemory = m_body->GetPosition();
	if (rand() % 2)
	{
		m_body->SetTransform(Screen2World({ SCREEN_WIDTH - BOSS_WIDTH / 2 ,SCREEN_HEIGHT / 4 }), 0);
		m_body->SetLinearVelocity({ -10.f,0 });
	}
	else
	{
		m_body->SetTransform(Screen2World({ BOSS_WIDTH / 2,SCREEN_HEIGHT / 4 }), 0);
		m_body->SetLinearVelocity({ 10.f,0 });
	}
	m_body->SetGravityScale(0);

	B2Dinfo tempBody;

	tempBody.density = 1;
	tempBody.fixedRotation = true;
	tempBody.friction = 0;
	tempBody.points =
	{
		b2Vec2(-LASER_WIDTH_B2D / 2, -LASER_HEIGHT_B2D),
		b2Vec2(-LASER_WIDTH_B2D / 2, 0),
		b2Vec2(LASER_WIDTH_B2D / 2, 0),
		b2Vec2(LASER_WIDTH_B2D / 2, -LASER_HEIGHT_B2D)
	};
	tempBody.shape = POLYGON;
	tempBody.type = b2_kinematicBody;
	tempBody.pos = sf::Vector2f{ m_body->GetPosition().x,m_body->GetPosition().y - BOSS_HEIGHT / (float)ZOOM / 2.f + 1 };
	tempBody.userData.fixtureName = "Laser";
	tempBody.userData.interruptGrapin = false;

	m_laser = CreateBodyB2D(_world, tempBody);
	m_laser->SetGravityScale(0);
	m_laser->GetFixtureList()->SetSensor(true);

	m_cristalData->CreateCristal(_world, Screen2World({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 * 3 }));

	m_doMove = true;
}

void BossDeath::CometExploInit(b2Vec2 _pos, b2World* _world)
{
	sf::Vector2f bodyPos = World2Screen(_pos);
	bodyPos.y -= 80;
	m_cometAnim->timeAnim = 0;
	b2Body* projectileBody;
	B2Dinfo tempBody;
	tempBody.userData.fixtureName = "ExploComet";
	tempBody.type = b2_dynamicBody;
	tempBody.shape = POLYGON;
	tempBody.pos = { bodyPos.x / 64.f, -bodyPos.y / 64.f };
	tempBody.points =
	{
		b2Vec2(-BOSS_HEIGHT / 128, BOSS_WIDTH / 128),
		b2Vec2(-BOSS_HEIGHT / 128,-BOSS_WIDTH / 128),
		b2Vec2(BOSS_HEIGHT / 128, -BOSS_WIDTH / 128),
		b2Vec2(BOSS_HEIGHT / 128,  BOSS_WIDTH / 128),
	};
	tempBody.density = 1;
	tempBody.friction = 1;
	projectileBody = CreateBodyB2D(_world, tempBody);

	projectileBody->SetGravityScale(0);
	projectileBody->GetFixtureList()->SetSensor(true);
	m_ExploCometBody = projectileBody;
	m_timerCometExplo = TIMER_COMET_EXPLO;
}

void BossDeath::BlastCenterInit(b2World* _world) //Swann
{
	B2Dinfo tempBody;

	m_posMemory = m_body->GetPosition();

	Teleport(Screen2World({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }), false);
	//m_body->SetTransform(Screen2World({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }), 0);

	m_explosionTimer = EXPLOSION_TIMER;

	tempBody.userData.fixtureName = "Explosion";
	tempBody.userData.interruptGrapin = false;
	tempBody.type = b2_staticBody;
	tempBody.shape = CIRCLE;
	tempBody.radius = 0.5f;
	tempBody.pos = Screen2WorldVec({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });

	m_explosion = CreateBodyB2D(_world, tempBody);
	m_explosion->GetFixtureList()->SetSensor(true);

	m_exploded = false;

	m_cristalData->CreateCristal(_world, Screen2World({ (float)(rand() % SCREEN_WIDTH), SCREEN_HEIGHT / 5 * 4 }));

	m_doMove = false;
}
#pragma endregion

void BossDeath::MovementBoss(b2World* _world, float _dt)
{
	sf::Vector2f bossPos = { (float)m_body->GetPosition().x , (float)m_body->GetPosition().y };

	if (m_isGoingDown)
	{
		m_offSetVelocityY -= _dt * 100;
	}
	else
	{
		m_offSetVelocityY += _dt * 100;
	}

	if (m_offSetVelocityY >= BOSS_MAX_VELOCITY)
	{
		m_isGoingDown = true;
	}
	if (m_offSetVelocityY <= BOSS_MIN_VELOCITY)
	{
		m_isGoingDown = false;
	}
	if (m_direction == BOSS_LEFT_DIR)
	{
		m_body->SetLinearVelocity({ -1 , (((int)m_offSetVelocityY % BOSS_MAX_VELOCITY) - 100.f) / 100.f });
	}
	else
	{
		m_body->SetLinearVelocity({ 1,  (((int)m_offSetVelocityY % BOSS_MAX_VELOCITY) - 100.f) / 100.f });
	}


	if (bossPos.x <= BOSS_MIN_X)
	{
		if (m_direction == BOSS_LEFT_DIR)
		{
			m_direction = BOSS_RIGHT_DIR;
		}
	}
	else if (bossPos.x >= BOSS_MAX_X)
	{
		if (m_direction == BOSS_RIGHT_DIR)
		{
			m_direction = BOSS_LEFT_DIR;

		}
	}

}

void BossDeath::DeleteGhostTab(int _i, b2World* _world)
{
	_world->DestroyBody(m_ghostTab[_i]);
	m_ghostTab[_i] = nullptr;
}

void BossDeath::Teleport(b2Vec2 _destination, bool _isSided)
{
	m_posMemory = m_body->GetPosition();
	m_tpTarget = _destination;
	m_isTeleporting = true;
	m_tpTime = TIME_TO_TP;
	m_timerTp = 0;
}

void BossDeath::TpUpdate(float _dt)
{
	if (m_isTeleporting == true)
	{
		m_timerTp += _dt;
		if (m_timerTp >= m_tpTime)
		{
			m_isTeleporting = false;
			m_timerTp = m_tpTime;
			m_isTeleported = true;
			m_body->SetTransform(m_tpTarget, 0);
		}
		float x = m_timerTp / m_tpTime;

		//m_sprite.setScale({ tanf(m_timerTp/2 * M_PI), tanf(-m_timerTp/2 * M_PI) });

		// tkt c'est pour scale avec le temps
		// the formula for scale to x is: f(x) = -5.875 * (x-0.4125)² + 2
		// the formula for scale to y is: f(x) = 5.875 * (x-0.4125)²
		m_sprite.setScale({ -5.875f * ((x - 0.4125f) * (x - 0.4125f)) + 2,5.875f * ((x - 0.4125f) * (x - 0.4125f)) });
	}
	if (m_isTeleported == true)
	{
		m_timerTp -= _dt;
		if (m_timerTp <= 0)
		{
			m_timerTp = 0;
			m_isTeleported = false;
		}

		float x = m_timerTp / m_tpTime;

		// je réutilise la formule magique une nouvelle fois mais tkt
		m_sprite.setScale({ -5.875f * ((x - 0.4125f) * (x - 0.4125f)) + 2,5.875f * ((x - 0.4125f) * (x - 0.4125f)) });
	}
	//Fix sprite pixelated after tp
	if (m_isTeleported == false && m_isTeleporting == false)
	{
		m_sprite.setScale({ 1,1 });
	}
}

#pragma region Cristal manager
Cristal::Cristal(b2World* _world, b2Vec2 _pos, unsigned int _index)
{
	B2Dinfo tempBody;
	tempBody.userData.fixtureName = "Cristal" + std::to_string(_index);
	tempBody.userData.interruptGrapin = false;
	tempBody.type = b2_staticBody;
	tempBody.shape = CIRCLE;
	tempBody.radius = 0.5;
	tempBody.pos = { _pos.x, _pos.y };

	m_body = CreateBodyB2D(_world, tempBody);
	m_body->GetFixtureList()->SetSensor(true);
}

Cristal::~Cristal()
{

}

void CristalData::CristalExplode(b2World* _world, unsigned int _index, bool _createSafezone)
{
	if (_createSafezone)
	{
		CreateSafezone(_world, m_cristalList[_index]->GetCristalPos());
	}
	_world->DestroyBody(m_cristalList[_index]->GetBody());
	m_cristalList[_index] = nullptr;
}

CristalData::~CristalData()
{

}

CristalData::CristalData()
{
	m_spriteCristal = LoadSprite("Sprites/Boss/Death/Cristal.png", true);
	m_spriteSafezone = LoadSprite("Sprites/Boss/Death/Safezone.png", true);
}

void CristalData::Update(float _dt)
{

}

void CristalData::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < m_cristalList.size(); i++)
	{
		if (m_cristalList[i] != nullptr)
		{
			BlitSprite(m_spriteCristal, World2Screen(m_cristalList[i]->GetCristalPos()), 0, _window);
		}
	}
	for (int i = 0; i < m_safeZone.size(); i++)
	{
		BlitSprite(m_spriteSafezone, World2Screen(m_safeZone[i]->GetPosition()), 0, _window);
	}
}

void CristalData::CreateCristal(b2World* _world, b2Vec2 _pos)
{
	m_cristalList.push_back(new Cristal(_world, _pos, m_cristalList.size()));
}

void CristalData::CreateSafezone(b2World* _world, b2Vec2 _pos)
{
	B2Dinfo tempInfo;
	tempInfo.density = 1;
	tempInfo.fixedRotation = true;
	tempInfo.friction = 1;
	tempInfo.pos = { _pos.x, _pos.y };
	tempInfo.radius = SAFE_ZONE_RADIUS;
	tempInfo.shape = CIRCLE;
	tempInfo.type = b2_dynamicBody;
	tempInfo.userData.fixtureName = "Safezone";
	tempInfo.userData.interruptGrapin = false;
	b2Body* tempBody = CreateBodyB2D(_world, tempInfo);
	tempBody->GetFixtureList()->SetSensor(true);
	tempBody->SetGravityScale(0);
	m_safeZone.push_back(tempBody);
}

void CristalData::DestroyAllElement(b2World* _world)
{
	for (int i = 0; i < m_cristalList.size(); i++)
	{
		if (m_cristalList[i] != nullptr)
		{
			_world->DestroyBody(m_cristalList[i]->GetBody());
			m_cristalList[i] = nullptr;
		}
	}
	for (int i = 0; i < m_safeZone.size(); i++)
	{
		if (m_safeZone[i] != nullptr)
		{
			_world->DestroyBody(m_safeZone[i]);
			m_safeZone[i] = nullptr;
		}
	}
	m_cristalList.clear();
	m_safeZone.clear();
}
#pragma endregion