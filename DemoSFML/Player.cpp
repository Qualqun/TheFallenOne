#include "Player.h"

#define GRAPLING_SPEED 30
#define HOOKED_SPEED 35
#define SPEED 10
#define JUMP_HEIGHT 10
#define RADIUS_FOR_TP 1
#define TIME_INVICIBILITY 3
// Multiplie (this is the speed of execution of the hit back, we divide that by dt so in our case it will be executed in this case the speed of execution of the hit back is multiplied by 10 )
//(therefore the duration is 1/10 sec )
#define TIME_HITBACK (1.f / 6.f) // execution speed 1/10 sec


#define TIME_HIT_ENEMY 0.5
#define ENEMY_KNOCKBACK_X 15
#define ENEMY_KNOCKBACK_Y 20
#define ENEMY_KNOCKBACK_UP 25

#define SUCKED_FORCE 2

#define NB_ANIMATION 11

//Instant
#define WRATH_COOLDOWN 3
#define GREED_COOLDOWN 5
#define PRIDE_COOLDOWN 3
#define SLOTH_COOLDOWN 5
#define ENVY_COOLDOWN 1
#define GLUTTONY_COOLDOWN 5
#define LUST_COOLDOWN 7

#define SEGMENT_WHEEL (2 * M_PI) / NBR_POWER

enum AttackHit
{
	NO_ATTACK,
	NO_HIT,
	HIT
};


PlayerData::PlayerData(b2World* _world)
{
	B2Dinfo playerInfo;

	int32 nbPoints = 4;
	b2PolygonShape shape;
	b2Vec2* points = new b2Vec2[nbPoints];

	b2FixtureDef fixtureDef;

	m_sizeAttack = { 1.75,1 }; //In m_sizeAttack the x corresponds to the longest side, and the y the smallest (do not take it as x = width and y = height)
	m_sizeBody = { 1.5,2.4 };

	m_hp = MAX_HP;
	m_velocity = { 0,0 };
	m_state = NORMAL;
	m_moveDir = LEFT;
	m_dirAttack = LEFT;
	m_jump = true;
	m_timerHitedEnemy = 0;


	//Creation of player body
	playerInfo.type = b2_dynamicBody;
	playerInfo.shape = POLYGON;
	playerInfo.userData.fixtureName = "Player";
	playerInfo.pos = { 3,-9 };
	playerInfo.points.push_back(b2Vec2(0, 0));
	playerInfo.points.push_back(b2Vec2(m_sizeBody.x, 0));
	playerInfo.points.push_back(b2Vec2(m_sizeBody.x, m_sizeBody.y));
	playerInfo.points.push_back(b2Vec2(0, m_sizeBody.y));
	playerInfo.density = 1;
	playerInfo.friction = 0;
	playerInfo.fixedRotation = true;

	m_playerBody = CreateBodyB2D(_world, playerInfo, PLAYER, BOUNDARY | PIC);

	// creation of fixture for foot player
	points[0] = { 0.1f, -0.1f };
	points[1] = { m_sizeBody.x - 0.1f, -0.1f };
	points[2] = { m_sizeBody.x - 0.1f,  0.2f };
	points[3] = { 0.1f, 0.2f };
	shape.Set(points, nbPoints);

	fixtureDef.userData.pointer = (uintptr_t) new std::string("FootPlayer");
	fixtureDef.shape = &shape;
	fixtureDef.density = 1;
	fixtureDef.friction = 500;

	//add fixture at body
	m_playerBody->CreateFixture(&fixtureDef)->SetSensor(true);

	m_playerBody->SetGravityScale(2);

	// init
	m_attackBody = nullptr;
	m_attackTimer = 0;
	m_timerStep = 0;
	m_attackOffSetDir = { 0,0 };
	m_timerInvicibility = 0;
	m_spriteOffSett = { 0,0 };
	m_secondAttack = 0;
	m_timeSecondAttack = 0;

	//Init grapling
	m_graplingBody = nullptr;
	m_hookedPos = { 0,0 };
	m_offHooked = true;
	m_hung = false;

	m_chainSprite = LoadSprite("Sprites/Player/Chain.png", false);
	m_chainSprite.setOrigin(sf::Vector2f(0, m_chainSprite.getGlobalBounds().height / 2));

	m_shaderChain.loadFromFile("Shaders/Player/Chain.frag", sf::Shader::Fragment);
	m_shaderChain.setUniform("sizeSprite", m_chainSprite.getGlobalBounds().width);

	m_hookSprite = LoadSprite("Sprites/Player/GraplingEnd.png", false);
	m_hookSprite.setOrigin(sf::Vector2f(0, m_hookSprite.getGlobalBounds().height / 2));

	//Init player animations
	m_spriteSheet = LoadSprite("Sprites/Player/SpriteSheet.png", false);
	m_stateAnimation = RUN_RIGHT;
	m_anim = new Anim[TOTAL_PLAYER_ANIM];

	m_anim[IDLE_RIGHT] = InitAnimation(sf::Vector2i(332, 382), 5, 5, 0, true);
	m_anim[IDLE_LEFT] = InitAnimation(sf::Vector2i(332, 382), 5, 5, 0, true);
	m_anim[RUN_RIGHT] = InitAnimation(sf::Vector2i(332, 382), 12, 7, 0, true);
	m_anim[RUN_LEFT] = InitAnimation(sf::Vector2i(332, 382), 12, 7, 0, true);

	m_anim[ATTACK_RIGHT_ONE] = InitAnimation(sf::Vector2i(332, 382), 20, 5, 0, false);
	m_anim[ATTACK_RIGHT_TWO] = InitAnimation(sf::Vector2i(332, 382), 20, 7, 0, false);

	m_anim[ATTACK_LEFT_ONE] = InitAnimation(sf::Vector2i(332, 382), 20, 5, 0, false);
	m_anim[ATTACK_LEFT_TWO] = InitAnimation(sf::Vector2i(332, 382), 20, 7, 0, false);

	m_anim[ATTACK_TOP] = InitAnimation(sf::Vector2i(332, 382), 20, 9, 0, false);

	m_anim[ATTACK_AIR_RIGHT] = InitAnimation(sf::Vector2i(332, 382), 20, 6, 0, false);
	m_anim[ATTACK_AIR_LEFT] = InitAnimation(sf::Vector2i(332, 382), 20, 6, 0, false);
	m_anim[ATTACK_AIR_TOP] = InitAnimation(sf::Vector2i(332, 382), 20, 9, 0, false);

	m_anim[ATTACK_BOT] = InitAnimation(sf::Vector2i(332, 382), 25, 11, 0, false);

	m_anim[JUMP_RIGHT] = InitAnimation(sf::Vector2i(332, 382), 12, 3, 0, true);
	m_anim[JUMP_LEFT] = InitAnimation(sf::Vector2i(332, 382), 12, 3, 0, true);

	m_anim[TRANSITION_RIGHT] = InitAnimation(sf::Vector2i(332, 382), 12, 3, 0, false);
	m_anim[TRANSITION_LEFT] = InitAnimation(sf::Vector2i(332, 382), 12, 3, 0, false);

	m_anim[FALL_RIGHT] = InitAnimation(sf::Vector2i(332, 382), 12, 3, 0, true);
	m_anim[FALL_LEFT] = InitAnimation(sf::Vector2i(332, 382), 12, 3, 0, true);

	m_anim[FALL_POWER] = InitAnimation(sf::Vector2i(100, 268), 2, 1, 0, true);

	CreatePowerData(IP_WRATH, 0, "FALL", sf::Color(237, 34, 13));//RED
	m_wrathPowerSprite = LoadSprite("Sprites/PowerUps/Power_Wrath.png", true);

	CreatePowerData(IP_GREED, 0, "REWIND", sf::Color(255, 129, 4));//ORANGE
	m_greedPowerSprite = LoadSprite("Sprites/PowerUps/Power_Greed.png", true);

	CreatePowerData(IP_PRIDE, 0, "RISE", sf::Color(255, 218, 49));//YELLOW
	m_pridePowerSprite = LoadSprite("Sprites/PowerUps/Power_Pride.png", true);

	CreatePowerData(IP_SLOTH, 0, "SHOT", sf::Color(59, 173, 14));//GREEN
	m_slothPowerSprite = LoadSprite("Sprites/PowerUps/Power_Sloth.png.", true);

	CreatePowerData(IP_ENVY, 0, "DASH", sf::Color(0, 161, 255));//BLUE
	m_envyPowerSprite = LoadSprite("Sprites/PowerUps/Power_Envy.png.", true);

	CreatePowerData(IP_GLUTTONY, 0, "COUNTER", sf::Color(148, 33, 146));//PURPLE
	m_gluttonyPowerSprite = LoadSprite("Sprites/PowerUps/Power_Gluttony.png.", true);

	CreatePowerData(IP_LUST, 0, "REVEAL", sf::Color(255, 66, 161));//PINK
	m_lustPowerSprite = LoadSprite("Sprites/PowerUps/Power_Lust.png.", true);

	m_powerSpriteVec.push_back(m_wrathPowerSprite);
	m_powerSpriteVec.push_back(m_greedPowerSprite);
	m_powerSpriteVec.push_back(m_pridePowerSprite);
	m_powerSpriteVec.push_back(m_slothPowerSprite);
	m_powerSpriteVec.push_back(m_envyPowerSprite);
	m_powerSpriteVec.push_back(m_gluttonyPowerSprite);
	m_powerSpriteVec.push_back(m_lustPowerSprite);

	m_shotSprite = LoadSprite("Sprites/Player/EnergyBall.png", true);

	m_policeLust.loadFromFile("Font/Chapbook-Regular.otf");    //Chargement du fichier de police (provenant de dafont.com)
	m_text.setFont(m_policeLust);                  //Assignation de la police au texte

	m_tabSentencePandemineP1.push_back("You dare oppose to the gods.");
	m_tabSentencePandemineP1.push_back("I, Pandemine, will stop you right there.");
	m_tabSentencePandemineP1.push_back("Can't you just die already ?");

	m_tabSentencePandemineP2.push_back("I can still see you.");
	m_tabSentencePandemineP2.push_back("You won't go far in my stomach.");
	m_tabSentencePandemineP2.push_back("You can't escape from here.");

	m_tabSentenceJumpLevel.push_back("What happened to you, Pandemine ?");
	m_tabSentenceJumpLevel.push_back("How could you end this way...");
	m_tabSentenceJumpLevel.push_back("I can't believe it.");

	m_tabSentenceReapwarP1.push_back("Who the hell are you suppose to be.");
	m_tabSentenceReapwarP1.push_back("I can't believe a weak Human defeated Pandemine.");
	m_tabSentenceReapwarP1.push_back("I will avenge my brother.");

	m_tabSentenceReapwarP2.push_back("I tried to warn you, you can't win.");
	m_tabSentenceReapwarP2.push_back("My... Power... It's overflowing.");
	m_tabSentenceReapwarP2.push_back("You just reached your end.");

	m_tabSentenceTuto.push_back("... Stop ...");
	m_tabSentenceTuto.push_back("... That Hurts ...");
	m_tabSentenceTuto.push_back("... Why ...");
}


void PlayerData::Update(GameData& _gameData, float _dt, b2World* _world, sf::RenderWindow& _window)
{
	float frameTime;

	m_timerDamage -= _dt;
	m_changing -= _dt;
	m_noMouvement -= _dt;
	m_noHit -= _dt;
	m_noHitAndHeal -= _dt;
	m_timeBetweenRewind -= _dt;
	m_timeSecondAttack -= _dt; 

	for (int i = 0; i < NBR_POWER; i++)
	{
		m_powerData[i].m_cooldown -= _dt;
	}

	frameTime = 1.f / m_anim[m_stateAnimation].framerate;

	// sound check to know if we hit or not 
	if (m_attackStatus == HIT)
	{
		int soundRand = rand() % 3;
		_gameData.soundData->UseSound(SOURCE_PLAYER, soundRand);
		m_attackStatus = NO_ATTACK;
	}
	else if (m_attackStatus == NO_HIT)
	{
		int soundRand = NO_HIT_ONE + rand() % 3;
		_gameData.soundData->UseSound(SOURCE_PLAYER, soundRand);
		m_attackStatus = NO_ATTACK;
	}

	UpdateEvntActionsPlayer(_world, *_gameData.soundData);

	switch (m_state)
	{
	case NORMAL:
		m_velocity = m_playerBody->GetLinearVelocity();
		m_hung = false;

		UpdateDirection();
		Move(_dt, *_gameData.particleData);

		break;
	case HIT_BACK:
		Hit(_dt);
		break;

	case HITED:
		m_velocity = m_playerBody->GetLinearVelocity();
		Hited(_dt);

		

		

		break;
	case HOOKED:
		HookedUpdate(_dt);
		break;
	default:
		break;
	}

	Power(_gameData, _window, _world);

	m_playerBody->SetLinearVelocity(m_velocity);
	m_playerBody->SetFixedRotation(true);

	if (m_playerBody->GetLinearVelocity().y <= -2)
	{
		m_jump = false;

		if (m_stateAnimation == IDLE_RIGHT || m_stateAnimation == RUN_RIGHT || m_stateAnimation == JUMP_RIGHT)
		{
			m_stateAnimation = TRANSITION_RIGHT;
		}
		else if (m_stateAnimation == IDLE_LEFT || m_stateAnimation == RUN_LEFT || m_stateAnimation == JUMP_LEFT)
		{
			m_stateAnimation = TRANSITION_LEFT;
		}
	}

	if ((m_stateAnimation == JUMP_RIGHT || m_stateAnimation == JUMP_LEFT) && m_playerBody->GetLinearVelocity().y == 0)
	{

		m_jump = true;
		m_stateAnimation = IDLE_RIGHT;
	}
	
	std::cout << m_playerBody->GetLinearVelocity().y << std::endl;

	if (m_attackBody != nullptr)
	{
		m_attackTimer += _dt;
		m_attackBody->SetTransform(b2Vec2(m_playerBody->GetPosition().x + m_attackOffSetDir.x, m_playerBody->GetPosition().y + m_attackOffSetDir.y), 0);

		if (m_attackTimer > 0.15)
		{
			_world->DestroyBody(m_attackBody);
			m_attackBody = nullptr;
			m_attackTimer = 0;
		}
	}

	if (m_graplingBody != nullptr)
	{
		b2Vec2 velocity;
		velocity.x = cosf(m_dirGrapling) * GRAPLING_SPEED;
		velocity.y = -sinf(m_dirGrapling) * GRAPLING_SPEED;

		m_graplingBody->SetLinearVelocity(velocity);
	}

	Animation(m_spriteSheet, m_anim, m_stateAnimation + m_secondAttack, _dt);
	m_spriteSheet.setOrigin(sf::Vector2f(m_spriteSheet.getGlobalBounds().width / 2, m_spriteSheet.getGlobalBounds().height -120));


	for (int i = 0; i < m_animChain.size(); i++)
	{
		Animation(m_animChain[i].second, m_animChain[i].first, 0, _dt);
	}

	if (m_anim->timeAnim >= frameTime * m_anim[m_stateAnimation].frameNb)
	{
		m_stateAnimation = IDLE_RIGHT;
	}

	if (m_timerInvicibility >= 0)
	{
		m_timerInvicibility -= _dt;
	}

	if (m_timerStep >= 0.35)
	{
		int soundRand = DIRT_ONE + rand() % 3;
		_gameData.soundData->UseSound(SOURCE_PLAYER, soundRand);

		m_timerStep = 0;
	}

	m_playerBody->SetAwake(true);
}

void::PlayerData::Power(GameData& _gameData, sf::RenderWindow& _window, b2World* _world)
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::X) && m_clickSwitch != 0)
	{
		if (m_clickSwitch <= 7)
		{
			m_changing = 0.5;
			SetSelectedPower(GetSelectedPower() + 1);
			if (GetSelectedPower() >= m_powerData.size())
			{
				SetSelectedPower(0);
			}
		}
		m_clickSwitch = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		if (m_clickSwitch > 7)
		{
			sf::Vector2i tempPosMouse;
			sf::Vector2f tempPosPlayer;

			tempPosMouse = sf::Mouse::getPosition(_window) + sf::Vector2i(_gameData.camData->GetPosition() - sf::Vector2f{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }) - (sf::Vector2i)World2ScreenVec(m_sizeBody / 2.0f);
			tempPosPlayer = World2Screen(m_playerBody->GetPosition());

			sf::Vector2f tempMousePlayer(tempPosMouse.y - tempPosPlayer.y, tempPosMouse.x - tempPosPlayer.x);
			float tempNorm = sqrtf(powf(tempMousePlayer.x, 2) + powf(tempMousePlayer.y, 2));
			tempMousePlayer.x /= tempNorm;
			tempMousePlayer.y /= tempNorm;

			float angle = atan2f(tempPosMouse.y - tempPosPlayer.y, tempPosMouse.x - tempPosPlayer.x);
			angle += M_PI - 5 * M_PI / (NBR_POWER * 2);

			float maxI = -10;
			float maxDot = -10;
			for (int i = 0; i < NBR_POWER; i++)
			{
				sf::Vector2f tempVec;
				tempVec.x = -cosf(SEGMENT_WHEEL * i);
				tempVec.y = sinf(SEGMENT_WHEEL * i);

				float tempDot = ((tempVec.x * tempMousePlayer.x) + (tempVec.y * tempMousePlayer.y));
				if (tempDot > maxDot)
				{
					maxDot = tempDot;
					maxI = i;
				}
			}
			if (maxI != -10)
			{
				SetSelectedPower(maxI);
			}
		}
		else
		{
			m_clickSwitch += 1;
		}
	}

	if (m_timerInvicibility <= 0)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && m_state != HOOKED)
		{
			m_clickSwitch = 0;

			if (m_powerData[GetSelectedPower()].m_enum == IP_WRATH && m_powerData[GetSelectedPower()].m_cooldown <= 0 && !m_canSuperFall)
			{
				for (size_t i = 0; i < m_sizeBody.x * 10; i++)
				{
					_gameData.particleData->CreatePowerParticle(World2Screen(b2Vec2(m_playerBody->GetPosition().x + (i / 10), m_playerBody->GetPosition().y)), 0.25, m_powerData[GetSelectedPower()].m_color, 1);
				}
				m_noMouvement = 0.25;
				m_velocity.x /= 3;
				m_velocity.y /= 2;
				m_canSuperFall = true;
			}

			if (m_powerData[GetSelectedPower()].m_enum == IP_GREED && m_powerData[GetSelectedPower()].m_cooldown <= 0)
			{
				m_powerData[GetSelectedPower()].m_cooldown = GREED_COOLDOWN;
				m_canRewind = true;
			}

			if (m_powerData[GetSelectedPower()].m_enum == IP_PRIDE && m_powerData[GetSelectedPower()].m_cooldown <= 0 && !m_canSuperJump)
			{
				m_noMouvement = 0.25;
				m_velocity.x /= 3;
				m_velocity.y /= 2;
				m_canSuperJump = true;
			}

			if (m_powerData[GetSelectedPower()].m_enum == IP_SLOTH && m_powerData[GetSelectedPower()].m_cooldown <= 0)
			{
				B2Dinfo shotInfo;
				m_sizeAttackShot = { 1 , 1 };
				shotInfo.type = b2_dynamicBody;
				shotInfo.shape = POLYGON;
				shotInfo.userData.fixtureName = "ShotSLOTH";
				shotInfo.points.push_back(b2Vec2(0, 0));
				shotInfo.points.push_back(b2Vec2(0, m_sizeAttackShot.y));
				shotInfo.points.push_back(b2Vec2(m_sizeAttackShot.x, m_sizeAttackShot.y));
				shotInfo.points.push_back(b2Vec2(m_sizeAttackShot.x, 0));
				shotInfo.density = 0;
				shotInfo.friction = 1;

				if (m_moveDir == LEFT)
				{
					_gameData.particleData->CreatePowerParticle(World2Screen(b2Vec2(m_playerBody->GetPosition().x - m_sizeBody.x, m_playerBody->GetPosition().y + m_sizeBody.y / 2)), 0.25, m_powerData[GetSelectedPower()].m_color, 10);
					shotInfo.pos = sf::Vector2f(m_playerBody->GetPosition().x - m_sizeBody.x * 2, m_playerBody->GetPosition().y + m_sizeBody.y / 3);
					m_shotBodySloth = CreateBodyB2D(_world, shotInfo);
					m_shotBodySloth->GetFixtureList()[0].SetSensor(true);
					m_shotBodySloth->SetLinearVelocity({ -25 , 0 });

					m_velocity.x = SPEED * 1.5f;
					m_noMouvement = 0.1f;
				}
				if (m_moveDir == RIGHT)
				{
					_gameData.particleData->CreatePowerParticle(World2Screen(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x, m_playerBody->GetPosition().y + m_sizeBody.y / 2)), 0.25, m_powerData[GetSelectedPower()].m_color, 10);
					shotInfo.pos = sf::Vector2f(m_playerBody->GetPosition().x + m_sizeBody.x * 2, m_playerBody->GetPosition().y + m_sizeBody.y / 3);
					m_shotBodySloth = CreateBodyB2D(_world, shotInfo);
					m_shotBodySloth->GetFixtureList()[0].SetSensor(true);
					m_shotBodySloth->SetLinearVelocity({ 25 , 0 });

					m_velocity.x = -SPEED * 1.5f;
					m_noMouvement = 0.1f;
				}

				m_powerData[GetSelectedPower()].m_cooldown = SLOTH_COOLDOWN;
			}

			if (m_powerData[GetSelectedPower()].m_enum == IP_ENVY && m_powerData[GetSelectedPower()].m_cooldown <= 0)
			{
				for (size_t i = 0; i < m_sizeBody.y * 10; i++)
				{
					_gameData.particleData->CreatePowerParticle(World2Screen(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + (i / 10))), 0.25, m_powerData[GetSelectedPower()].m_color, 1);
				}

				if (m_moveDir == LEFT)
				{
					m_velocity.x = -SPEED * 7;
				}
				if (m_moveDir == RIGHT)
				{
					m_velocity.x = SPEED * 7;
				}
				m_noMouvement = 0.1;
				m_noHit = 0.1;
				m_powerData[GetSelectedPower()].m_cooldown = ENVY_COOLDOWN;

			}

			if (m_powerData[GetSelectedPower()].m_enum == IP_GLUTTONY && m_powerData[GetSelectedPower()].m_cooldown <= 0)
			{
				sf::Vector2f tempPos;
				sf::Vector2f playerPos = World2Screen(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2));
				for (size_t i = 0; i < 36; i++)
				{
					tempPos.x = -cosf(10 * i) * 125;
					tempPos.y = -sinf(10 * i) * 125;
					_gameData.particleData->CreatePowerParticle(sf::Vector2f(playerPos.x + tempPos.x, playerPos.y + tempPos.y), 0.5, m_powerData[GetSelectedPower()].m_color, 1);
				}
				m_noHitAndHeal = 0.5;
				m_noMouvement = 1;
				m_velocity.x = 0;
				m_velocity.y = 0;
				m_powerData[GetSelectedPower()].m_cooldown = GLUTTONY_COOLDOWN;
			}

			if (m_powerData[GetSelectedPower()].m_enum == IP_LUST && m_powerData[GetSelectedPower()].m_cooldown <= 0)
			{
				TextInfo text;
				if (_gameData.mapData->GetNBBossMap() == SPAWN_JUMP_LEVEL)
				{
					text.m_string = m_tabSentenceJumpLevel[rand() % m_tabSentenceJumpLevel.size()];
				}
				else
				{
					switch (_gameData.currentBoss->GetGamePhase())
					{
					case PANDEMINE_FIRST_PHASE:
						text.m_string = m_tabSentencePandemineP1[rand() % 3];
						break;
					case PANDEMINE_SECOND_PHASE:
						text.m_string = m_tabSentencePandemineP2[rand() % 3];
						break;
					case REAPWAR_FIRST_PHASE:
						text.m_string = m_tabSentenceReapwarP1[rand() % 3];
						break;
					case REAPWAR_SECOND_PHASE:
						text.m_string = m_tabSentenceReapwarP2[rand() % 3];
						break;
					case TUTO_PHASE:
						text.m_string = m_tabSentenceTuto[rand() % 3];
						break;
					default:
						text.m_string = "...";
						break;
					}
				}
				text.m_size = 50;
				text.m_pos = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 7 };
				text.m_color = m_powerData[GetSelectedPower()].m_color;
				m_textLust = text;

				_gameData.particleData->CreatePowerParticle(World2Screen(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2 + m_sizeBody.y / 2)), 0.5, m_powerData[GetSelectedPower()].m_color, 5);

				m_powerData[GetSelectedPower()].m_cooldown = LUST_COOLDOWN;
			}
		}
	}

	if (m_canSuperFall && m_noMouvement < 0)
	{
		m_stateAnimation = FALL_POWER;

		m_velocity.y = -JUMP_HEIGHT * 2.5;
		m_powerData[GetSelectedPower()].m_cooldown = WRATH_COOLDOWN;
		m_canSuperFall = false;

		B2Dinfo fallInfo;
		m_sizeAttackFall = { 2.3 , 2.0 };
		fallInfo.type = b2_dynamicBody;
		fallInfo.shape = POLYGON;
		fallInfo.userData.fixtureName = "FallWRATH";
		fallInfo.points.push_back(b2Vec2(0, 0));
		fallInfo.points.push_back(b2Vec2(0, m_sizeAttackFall.y));
		fallInfo.points.push_back(b2Vec2(m_sizeAttackFall.x, m_sizeAttackFall.y));
		fallInfo.points.push_back(b2Vec2(m_sizeAttackFall.x, 0));
		fallInfo.density = 0;
		fallInfo.friction = 1;
		fallInfo.pos = sf::Vector2f(m_playerBody->GetPosition().x, m_playerBody->GetPosition().y);

		m_attackFallBody = CreateBodyB2D(_world, fallInfo);
		m_attackFallBody->GetFixtureList()[0].SetSensor(true);
	}

	if (m_attackFallBody != nullptr)
	{
		b2Vec2 targetPosition = { m_playerBody->GetPosition().x - (m_sizeAttackFall.x - m_sizeBody.x) / 2, m_playerBody->GetPosition().y - 1.4f };
		m_attackFallBody->SetTransform(targetPosition, m_attackFallBody->GetAngle());
		if (m_velocity.y == 0)
		{
			_world->DestroyBody(m_attackFallBody);
			m_attackFallBody = nullptr;
		}
	}

	if (m_timeBetweenRewind < 0 && m_vecPosForRewind.size() < NBR_CHECKPOINT_GREED && m_canRewind)
	{
		m_timeBetweenRewind = TIMER_SEGMENT_GREED;
		m_vecPosForRewind.push_back(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2));
		for (int i = 0; i < m_vecPosForRewind.size(); i++)
		{
			if (i % 2 == 0)
			{
				_gameData.particleData->CreatePowerParticle(World2Screen(m_vecPosForRewind[i]), TIMER_SEGMENT_GREED * 1.5f, sf::Color(255, 129, 4), 1);
			}
		}
	}
	else if ((m_timeBetweenRewind < 0 && m_vecPosForRewind.size() >= NBR_CHECKPOINT_GREED) || (m_timeBetweenRewind < 0 && m_isRewinding))
	{
		m_canRewind = false;
		m_timeBetweenRewind = TIMER_SEGMENT_GREED / 4;
		m_isRewinding = true;
		m_state = NORMAL;
		m_noMouvement = 0.1;
		m_velocity.x = 0;
		m_velocity.y = 0;

		auto it = m_vecPosForRewind.rbegin();
		b2Vec2 tempPoint = *it;
		m_playerBody->SetTransform(b2Vec2(tempPoint.x - m_sizeBody.x / 2, tempPoint.y - m_sizeBody.y / 2), 0);
		m_vecPosForRewind.erase(it.base() - 1);

		for (int i = 0; i < m_vecPosForRewind.size(); i++)
		{
			if (i % 2 == 0)
			{
				_gameData.particleData->CreatePowerParticle(World2Screen(m_vecPosForRewind[i]), (TIMER_SEGMENT_GREED * 1.5f), sf::Color(255, 129, 4), 1);
			}
		}

		if (m_vecPosForRewind.size() == 0)
		{
			m_isRewinding = false;
		}
	}

	if (m_canSuperJump && m_noMouvement < 0)
	{
		for (size_t i = 0; i < m_sizeBody.x * 10; i++)
		{
			_gameData.particleData->CreatePowerParticle(World2Screen(b2Vec2(m_playerBody->GetPosition().x + (i / 10), m_playerBody->GetPosition().y)), 0.25, m_powerData[GetSelectedPower()].m_color, 1);
		}

		m_jump = false;

		m_velocity.y = JUMP_HEIGHT * 2.0;
		m_powerData[GetSelectedPower()].m_cooldown = PRIDE_COOLDOWN;
		m_canSuperJump = false;
	}

	if (m_powerData[IP_SLOTH].m_cooldown <= 0 && m_shotBodySloth != nullptr)
	{
		DestroyShotSloth(_world);
	}

	if (m_noMouvement > 0)
	{
		m_playerBody->SetGravityScale(0);
	}
	else
	{
		m_playerBody->SetGravityScale(2);
	}
}

void PlayerData::Display(sf::RenderWindow& _window)
{
	sf::Vector2f playerPos = World2Screen(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2));

	if (m_graplingBody != NULL)
	{
		sf::Vector2f graplingPos = World2Screen(m_graplingBody->GetPosition());
		sf::Vector2f hookPos;
		float dir = atan2(graplingPos.y - playerPos.y, graplingPos.x - playerPos.x);
		float distanceX = graplingPos.x - playerPos.x;
		float distanceY = graplingPos.y - playerPos.y;

		float distance = sqrt(distanceX * distanceX + distanceY * distanceY);

		m_shaderChain.setUniform("hypotenuse", distance);

		BlitSprite(m_chainSprite, playerPos, RadToDeg(dir), _window, m_shaderChain);

		BlitSprite(m_hookSprite, graplingPos, RadToDeg(dir), _window);
	}
	else
	{
		m_animChain.clear();
	}



	if (m_shotBodySloth != nullptr)
	{
		m_shotSprite.setScale(0.75, 0.75);
		BlitSprite(m_shotSprite, World2Screen(b2Vec2(m_shotBodySloth->GetPosition().x + m_sizeAttackShot.x / 2, m_shotBodySloth->GetPosition().y + m_sizeAttackShot.y / 2)), 0, _window);
	}

	playerPos += m_spriteOffSett;

	m_spriteSheet.setColor(sf::Color(255, 255, 255, 255));
	if (m_timerInvicibility >= 0)
	{
		if (int(m_timerInvicibility * 10) % 2 == 0)
		{
			m_spriteSheet.setColor(sf::Color(255, 255, 255, 100));
		}
		else
		{
			m_spriteSheet.setColor(sf::Color(255, 255, 255, 255));
		}
	}

	BlitSprite(m_spriteSheet, playerPos, 0, _window);

}

void PlayerData::DisplayPower(sf::RenderWindow& _window)
{
	sf::Vector2f playerPos = World2Screen(b2Vec2(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2));

	if (m_clickSwitch > 7)
	{
		for (int i = 0; i < NBR_POWER; i++)
		{
			sf::Vector2f tempPos;
			tempPos.x = -cosf(SEGMENT_WHEEL * i + M_PI_2) * 125;
			tempPos.y = -sinf(SEGMENT_WHEEL * i + M_PI_2) * 125;

			if (m_powerData[GetSelectedPower()].m_enum == i)
			{
				m_powerSpriteVec[i].setScale(1.2, 1.2);

				sf::FloatRect textRect = m_text.getLocalBounds();
				m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				m_text.setString(m_powerData[GetSelectedPower()].m_name);           //Assignation d'un texte à afficher à l'écran
				m_text.setPosition(sf::Vector2f(playerPos.x + tempPos.x * 1.75, playerPos.y + tempPos.y * 1.75)); //Position pour l'affichage
				m_text.setFillColor(m_powerData[GetSelectedPower()].m_color);  //Couleur du texte
				m_text.setCharacterSize(25);
				_window.draw(m_text);
			}
			else
			{
				m_powerSpriteVec[i].setScale(1, 1);
			}

			if (m_powerData[i].m_cooldown > 0)
			{
				m_powerSpriteVec[i].setColor(sf::Color(100, 100, 100));
			}
			else
			{
				m_powerSpriteVec[i].setColor(sf::Color(255, 255, 255));
			}
			BlitSprite(m_powerSpriteVec[i], playerPos + tempPos, 0, _window);
		}
	}
	else if ((m_clickSwitch > 0 && m_clickSwitch <= 7 && !sf::Keyboard::isKeyPressed(sf::Keyboard::X)) || m_changing > 0)
	{
		sf::Vector2f tempPos;
		tempPos.x = 0;
		tempPos.y = -125;

		m_powerSpriteVec[GetSelectedPower()].setScale(1, 1);
		BlitSprite(m_powerSpriteVec[GetSelectedPower()], playerPos + tempPos, 0, _window);

		sf::FloatRect textRect = m_text.getLocalBounds();
		m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		m_text.setString(m_powerData[GetSelectedPower()].m_name);           //Assignation d'un texte à afficher à l'écran
		m_text.setPosition(sf::Vector2f(playerPos.x + tempPos.x, playerPos.y + tempPos.y - 60)); //Position pour l'affichage
		m_text.setFillColor(m_powerData[GetSelectedPower()].m_color);  //Couleur du texte
		m_text.setCharacterSize(25);
		_window.draw(m_text);
	}
}


void PlayerData::UpdateDirection()
{
	m_buttonPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		m_moveDir = TOP;
		m_dirAttack = TOP;
		m_buttonPressed = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_moveDir = DOWN;
		m_dirAttack = DOWN;
		m_buttonPressed = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_moveDir != DOWN && m_moveDir != TOP)
		{
			m_dirAttack = RIGHT;
		}
		m_moveDir = RIGHT;


		m_buttonPressed = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		if (m_moveDir != DOWN && m_moveDir != TOP)
		{
			m_dirAttack = LEFT;
		}
		m_moveDir = LEFT;
		m_buttonPressed = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_jump)
	{
		if (m_noMouvement < 0)
		{
			m_velocity.y = JUMP_HEIGHT;
			m_jump = false;
		}
		m_buttonPressed = true;
	}


}


void PlayerData::Move(float _dt, ParticleData& _particle)
{
	if (m_noMouvement < 0)
	{
		PlayerAnimation stateAnimTemp = IDLE_RIGHT;

		m_spriteOffSett = { 0,0 };

		if (m_moveDir == RIGHT && m_buttonPressed)
		{
			m_velocity.x = SPEED;
			m_offHooked = true;

			if (m_jump == false)
			{
				stateAnimTemp = JUMP_RIGHT;
			}
			else
			{
				stateAnimTemp = RUN_RIGHT;
				m_timerStep += _dt;
				_particle.CreateWalkDustParticle(World2Screen(m_playerBody->GetPosition()) + sf::Vector2f(rand() % (int)(m_sizeBody.x * ZOOM), 0), -1, sf::Color::White, 1, 1, false, COOLDOWN_SPAWN_WALK_DUST);
			}
		}
		else if (m_moveDir == LEFT && m_buttonPressed)
		{
			m_velocity.x = -SPEED;
			m_offHooked = true;

			if (m_jump == false)
			{
				stateAnimTemp = JUMP_LEFT;
			}
			else
			{
				stateAnimTemp = RUN_LEFT;
				m_timerStep += _dt;
				_particle.CreateWalkDustParticle(World2Screen(m_playerBody->GetPosition()) + sf::Vector2f(rand() % (int)(m_sizeBody.x * ZOOM), 0), 1, sf::Color::White, 1, 1, false, COOLDOWN_SPAWN_WALK_DUST);
			}
		}
		else if (m_offHooked)
		{
			if (m_jump == false)
			{
				if (m_stateAnimation == IDLE_RIGHT || m_stateAnimation == RUN_RIGHT || m_stateAnimation == JUMP_RIGHT)
				{
					stateAnimTemp = JUMP_RIGHT;
				}
				else
				{
					stateAnimTemp = JUMP_LEFT;
				}
			}
			else
			{
				if (m_moveDir)
				{
					stateAnimTemp = IDLE_RIGHT;
				}
				else
				{
					stateAnimTemp = IDLE_LEFT;
				}
			}
			m_velocity.x = 0;
		}

		if (!(m_stateAnimation <= ATTACK_BOT && m_stateAnimation >= ATTACK_RIGHT_ONE))
		{
			if (m_stateAnimation == TRANSITION_RIGHT || m_stateAnimation == TRANSITION_LEFT)
			{
				float frimeTime = 1.f / m_anim[m_stateAnimation].framerate;

				//We assume that the player will always be in the air so we take jump as a direction reference.
				if (m_anim->timeAnim >= frimeTime * m_anim[m_stateAnimation].frameNb)
				{
					//m_spriteOffSett.y -= 25;
					m_anim->timeAnim = 0;

					if (stateAnimTemp == TRANSITION_RIGHT)
					{
						m_stateAnimation = FALL_RIGHT;
					}
					else
					{
						m_stateAnimation = FALL_LEFT;
					}

				}
			}
			else if (m_stateAnimation != FALL_LEFT && m_stateAnimation != FALL_RIGHT || m_jump)
			{
				m_stateAnimation = stateAnimTemp;
			}
			else
			{
				//m_spriteOffSett.y -= 25;
				if (stateAnimTemp == JUMP_RIGHT)
				{
					m_stateAnimation = FALL_RIGHT;
				}
				else
				{
					m_stateAnimation = FALL_LEFT;

				}
			}
			m_secondAttack = 0;
		}

	}

	

	if (m_attackFallBody != nullptr)
	{
		m_stateAnimation = FALL_POWER;
	}

}

void PlayerData::Hit(float _dt)
{

	switch (m_dirAttack)
	{
	case LEFT:
		m_velocity.x -= HITBACK_LEFT * _dt / TIME_HITBACK;

		if (m_velocity.x < 0)
		{
			m_state = NORMAL;
		}

		break;
	case RIGHT:
		m_velocity.x += HITBACK_RIGHT * _dt / TIME_HITBACK;

		if (m_velocity.x > 0)
		{
			m_state = NORMAL;

		}
		break;

	case DOWN:
		m_velocity.y -= HITBACK_DOWN * _dt / TIME_HITBACK;
		if (m_velocity.y < 0)
		{
			m_state = NORMAL;
		}

		break;
	default:
		break;
	}
}

void PlayerData::Hited(float _dt)
{
	m_timerHitedEnemy += _dt;

	if (m_timerHitedEnemy >= TIME_HIT_ENEMY)
	{
		m_timerHitedEnemy = 0;
		m_state = NORMAL;
	}
}

void PlayerData::CanJump()
{
	m_jump = true;
	m_offHooked = true;
}

void PlayerData::UpdateEvntActionsPlayer(b2World* _world, SoundData& _sound)
{
	//In m_sizeAttack the x corresponds to the longest side, and the y the smallest (do not take it as x = width and y = height)
	if (m_evntActionPlayer == ATTACK && m_state != HOOKED)
	{
		b2Vec2 tempPlayerPos;
		B2Dinfo attackInfo;

		attackInfo.type = b2_dynamicBody;
		attackInfo.shape = POLYGON;
		attackInfo.userData.fixtureName = "Attack";

		switch (m_dirAttack)
		{
		case LEFT:
			attackInfo.points.push_back(b2Vec2(0, 0));
			attackInfo.points.push_back(b2Vec2(0, m_sizeAttack.y));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.x, m_sizeAttack.y));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.x, 0));

			m_attackOffSetDir.x = -m_sizeAttack.x;
			m_attackOffSetDir.y = m_sizeBody.y / 2 - m_sizeAttack.y / 2;
			m_anim->timeAnim = 0;

			if (m_jump)
			{
				m_stateAnimation = ATTACK_LEFT_ONE;
			}
			else
			{
				m_stateAnimation = ATTACK_AIR_LEFT;
			}

			break;
		case RIGHT:
			attackInfo.points.push_back(b2Vec2(0, 0));
			attackInfo.points.push_back(b2Vec2(0, m_sizeAttack.y));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.x, m_sizeAttack.y));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.x, 0));

			m_attackOffSetDir.x = m_sizeBody.x;
			m_attackOffSetDir.y = m_sizeBody.y / 2 - m_sizeAttack.y / 2;
			m_anim->timeAnim = 0;

			if (m_jump)
			{
				m_stateAnimation = ATTACK_RIGHT_ONE;
			}
			else
			{
				m_stateAnimation = ATTACK_AIR_RIGHT;
			}

			break;
		case TOP:
			attackInfo.points.push_back(b2Vec2(0, 0));
			attackInfo.points.push_back(b2Vec2(0, m_sizeAttack.x));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.y, m_sizeAttack.x));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.y, 0));

			m_attackOffSetDir.x = m_sizeBody.x / 2 - m_sizeAttack.y / 2;
			m_attackOffSetDir.y = m_sizeBody.y;
			m_anim->timeAnim = 0;

			if (m_jump)
			{
				m_stateAnimation = ATTACK_TOP;
			}
			else
			{
				m_stateAnimation = ATTACK_AIR_TOP;
			}


			break;
		case DOWN:
			// can attack down only if we are in the air
			if (m_jump)
			{
				return;
			}
			attackInfo.points.push_back(b2Vec2(0, 0));
			attackInfo.points.push_back(b2Vec2(0, m_sizeAttack.x));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.y, m_sizeAttack.x));
			attackInfo.points.push_back(b2Vec2(m_sizeAttack.y, 0));

			m_attackOffSetDir.x = m_sizeBody.x / 2 - m_sizeAttack.y / 2;
			m_attackOffSetDir.y = -m_sizeAttack.x;
			m_stateAnimation = ATTACK_BOT;
			m_anim->timeAnim = 0;

			break;
		default:
			break;
		}

		tempPlayerPos = m_playerBody->GetPosition();
		attackInfo.pos = sf::Vector2f(tempPlayerPos.x + m_attackOffSetDir.x, tempPlayerPos.y + m_attackOffSetDir.y);

		m_attackBody = CreateBodyB2D(_world, attackInfo);
		m_attackBody->GetFixtureList()[0].SetSensor(true);
		m_attackStatus = NO_HIT;


		if (m_stateAnimation == ATTACK_RIGHT_ONE || m_stateAnimation == ATTACK_LEFT_ONE)
		{
			if (m_timeSecondAttack > 0)
			{
				m_secondAttack = 1;
			}
			else
			{
				m_secondAttack = 0;
			}

			if (m_secondAttack == 0 && m_timeSecondAttack <= 0)
			{
				m_timeSecondAttack = 1;
			}
			else
			{
				m_timeSecondAttack = 0;
			}
		}


	}

	if (m_evntActionPlayer == GRAPLING)
	{
		b2Vec2 velocity;

		B2Dinfo graplingInfo;
		graplingInfo.type = b2_dynamicBody;
		graplingInfo.shape = CIRCLE;
		graplingInfo.userData.fixtureName = "Grapling";
		graplingInfo.radius = 0.25f;
		graplingInfo.pos = sf::Vector2f(m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2);


		m_graplingBody = CreateBodyB2D(_world, graplingInfo);
		m_graplingBody->GetFixtureList()[0].SetSensor(true);

		velocity.x = cosf(m_dirGrapling) * GRAPLING_SPEED;
		velocity.y = -sinf(m_dirGrapling) * GRAPLING_SPEED;

		m_graplingBody->SetLinearVelocity(velocity);
		_sound.UseSound(SOURCE_PLAYER, SEND_HOOK);
	}

	m_evntActionPlayer = NO_ACTIONS;
}


void PlayerData::MouseButtonPressed(b2World* _world, sf::Vector2f _posCameraCenter, sf::RenderWindow& _window)
{

	if (m_state != HITED)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_attackBody == nullptr)
		{
			m_evntActionPlayer = ATTACK;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_graplingBody == nullptr)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
			sf::Vector2f posGrapling = { m_playerBody->GetPosition().x + m_sizeBody.x / 2, m_playerBody->GetPosition().y + m_sizeBody.y / 2 };

			_posCameraCenter.x -= SCREEN_WIDTH / 2;
			_posCameraCenter.y -= SCREEN_HEIGHT / 2;
			mousePos += (sf::Vector2i)_posCameraCenter;

			m_dirGrapling = atan2f(mousePos.y - World2Screen(b2Vec2(posGrapling.x, posGrapling.y), sf::Vector2f(0, 0)).y, mousePos.x - World2Screen(b2Vec2(posGrapling.x, posGrapling.y), sf::Vector2f(0, 0)).x);

			m_evntActionPlayer = GRAPLING;
		}
	}
}

void PlayerData::DeleteGrappling(b2World* _world)
{
	if (m_graplingBody != nullptr)
	{
		_world->DestroyBody(m_graplingBody);
		m_graplingBody = nullptr;
	}
}

void PlayerData::Hooked(b2Vec2 _pos, SoundData* _sound)
{
	m_hookedPos = _pos;
	m_state = HOOKED;
	_sound->UseSound(SOURCE_PLAYER, HIT_HOOK);
}

void PlayerData::HookedUpdate(float _dt)
{
	float dir;
	b2Vec2 velocity;
	b2Vec2 centerPlayer = { m_playerBody->GetPosition().x + m_sizeBody.x / 2,  m_playerBody->GetPosition().y + m_sizeBody.y / 2 };


	if (!CollideCirclePoint(m_hookedPos, RADIUS_FOR_TP, centerPlayer))
	{
		dir = atan2f(-(m_hookedPos.y - centerPlayer.y), m_hookedPos.x - centerPlayer.x);

		m_velocity.x = cosf(dir) * HOOKED_SPEED;
		m_velocity.y = -sinf(dir) * HOOKED_SPEED;
	}
	else
	{
		b2Vec2 playerOnHookPos = { m_hookedPos.x - m_sizeBody.x / 2, m_hookedPos.y - m_sizeBody.y / 2 };
		m_playerBody->SetTransform(playerOnHookPos, 0);
	}

	if (m_hung)
	{
		m_velocity.y = m_playerBody->GetLinearVelocity().y / 1.5;
		m_velocity.x = m_playerBody->GetLinearVelocity().x / 1.5;

		m_jump = false;
		m_offHooked = false;
		m_state = NORMAL;

	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_velocity.y = m_playerBody->GetLinearVelocity().y / 1.5;
			m_velocity.x = m_playerBody->GetLinearVelocity().x / 1.5;

			m_jump = false;
			m_offHooked = false;
			m_state = NORMAL;

		}
	}
}

void PlayerData::EnemyHitRight()
{
	m_state = HITED;
	m_playerBody->SetLinearVelocity(b2Vec2(0, 0));
	m_playerBody->ApplyLinearImpulseToCenter(b2Vec2(ENEMY_KNOCKBACK_X, ENEMY_KNOCKBACK_Y), true);
}

void PlayerData::EnemyHitLeft()
{
	m_state = HITED;

	m_playerBody->SetLinearVelocity(b2Vec2(0, 0));
	m_playerBody->ApplyLinearImpulseToCenter(b2Vec2(-ENEMY_KNOCKBACK_X, ENEMY_KNOCKBACK_Y), true);
}

void PlayerData::EnemyHitUp()
{
	m_state = HITED;
	m_playerBody->SetLinearVelocity(b2Vec2(0, 0));
	m_playerBody->ApplyLinearImpulseToCenter(b2Vec2(0, ENEMY_KNOCKBACK_UP), true);
}

void PlayerData::Sucked(b2Vec2 _bossPos)
{
	if (m_state != HIT_BACK && m_state != HITED)
	{

		float suckedDir = atan2(_bossPos.y - m_playerBody->GetPosition().y, _bossPos.x - m_playerBody->GetPosition().x);

		m_velocity = m_playerBody->GetLinearVelocity();

		m_velocity.x += cos(suckedDir) * SUCKED_FORCE;

		m_playerBody->SetLinearVelocity(m_velocity);
	}
}

void PlayerData::TakeDamage(int _nbDamage, int _dirBump, SoundData& _sound)
{
	if (m_timerInvicibility <= 0)
	{
		if (m_noHit <= 0 && m_noHitAndHeal <= 0)
		{
			_sound.UseSound(SOURCE_PLAYER, TAKE_DAMAGE);
			m_hp -= _nbDamage;
		}
		else if (m_noHitAndHeal > 0 && m_hp <= MAX_HP)
		{
			m_hp += _nbDamage;
		}
		m_timerInvicibility = TIME_INVICIBILITY;

		_sound.UseSound(SOURCE_PLAYER, TAKE_DAMAGE);
		m_hp -= _nbDamage;
		m_timerInvicibility = TIME_INVICIBILITY;

		switch (_dirBump)
		{
		case BUMP_RIGHT:
			EnemyHitRight();
			break;
		case BUMP_LEFT:
			EnemyHitLeft();
			break;
		case BUMP_UP:
			EnemyHitUp();
			break;
		default:
			break;
		}
		m_timerDamage = 1.2f;
	}
	
}

b2Vec2 PlayerData::GetGraplingPos()
{
	if (m_graplingBody != nullptr)
	{
		return m_graplingBody->GetPosition();
	}

	std::cout << "Error position grapling" << std::endl;
}

void PlayerData::SetState(PlayerState _state)
{
	m_state = _state;

	if (_state == HIT_BACK)
	{
		m_attackStatus = HIT;
	}
}


void PlayerData::CreatePowerData(InstantPower _enum, float _cooldown, std::string _name, sf::Color _color)
{
	PowerData tempPowerData;
	tempPowerData.m_enum = _enum;
	tempPowerData.m_name = _name;
	tempPowerData.m_cooldown = _cooldown;
	tempPowerData.m_color = _color;
	AddPowerData(tempPowerData);
}

TextInfo PlayerData::GetTextInfo()
{
	if (m_textLust.m_string != "No Name")
	{
		TextInfo tempTextInfo = m_textLust;
		m_textLust.m_string = "No Name";
		return tempTextInfo;
	}
	return m_textLust;
}