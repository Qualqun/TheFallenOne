#include "BossFamine.h"

#define TIME_SUCTION 8
#define TIME_PREPARE_SUCTION 1

#define TIME_GROUNDATTACK 4
#define TIME_PREPARE_GROUNDATTACK 1.75
#define GROUND_PROJECTILE_SPEED 8
#define OFFSET_ATTACK_X 2

#define TIME_STRAIGHT_ARM 4
#define TIME_CHARGE_ARM 3
#define SPEED_ARM 10

#define DISTANCE_MAX_INTERACT 3

#define SPEED_DISPLAY_LETER 0.025

enum Dir
{
	LEFT = -1,
	RIGHT = 1
};

enum FaminePaterns
{
	NOTHING,
	SUCTION,
	GROUND_ATTACK,
	STRAIGHT_ARM,
	END_FIGHT
};

enum SpritesPaterns
{
	SEED,
	SEED_GROW,
	EXPLOSION
};

enum AllStep
{
	MOVE,
	JUMP,
	ATTACK,
	SELECT_POWER,
	USE_POWER,
	GRAB,
	HUNG,
	BEFOR_FIGHT,
	FIGHT
};

BossFamine::BossFamine(b2World* _world, sf::Vector2f _worldPos)
{
	m_gamePhase = TUTO_PHASE;

	B2Dinfo bossInfo;
	std::string text;

	m_grabed = false;

	bossInfo.type = b2_kinematicBody;
	bossInfo.shape = POLYGON;
	bossInfo.userData.fixtureName = "FamineBoss";
	bossInfo.userData.interruptGrapin = false;
	bossInfo.pos = sf::Vector2f(_worldPos.x, _worldPos.y);
	bossInfo.points.push_back(b2Vec2(0, 0));
	bossInfo.points.push_back(b2Vec2(BOSS_SIZE_X, 0));
	bossInfo.points.push_back(b2Vec2(BOSS_SIZE_X, BOSS_SIZE_Y));
	bossInfo.points.push_back(b2Vec2(0, BOSS_SIZE_Y));
	bossInfo.density = 1;
	bossInfo.friction = 1;
	bossInfo.fixedRotation = true;

	m_body = CreateBodyB2D(_world, bossInfo);

	m_globalTimer = 3;
	m_hp = 100;
	m_currentPatern = NOTHING;
	m_gamePhase = TUTO_PHASE;

	m_spriteHelice = LoadSprite("Sprites/Map/Famine/Helice.png", true);
	m_groundFront = LoadSprite("Sprites/Map/Famine/GroundTuto.png", false);

	m_bodySuction = nullptr;

	m_straightBody = nullptr;
	m_tempShowPatern = SUCTION;

	m_dir = 0;
	m_dirGround = 0;
	m_dirStraightAttack = 0;
	m_groundAttack = nullptr;

	//load all sprites
	m_paternsSprites[PROJECTILE] = LoadSprite("Sprites/Boss/Famine/Projectile.png", true);

	m_paternsSprites[BOSS_SPRITE_FAMINE] = LoadSprite("Sprites/Boss/Famine/DummySpritesheet.png", false);

	m_animDummy[SUCTION_DUMMY] = InitAnimation(sf::Vector2i(400, 200), 20, 24, 0, true);
	m_animDummy[GROUND_DUMMY] = InitAnimation(sf::Vector2i(400, 200), 20, 24, 0, false);
	m_animDummy[STRAIGHT_DUMMY] = InitAnimation(sf::Vector2i(400, 200), 20, 20, 0, false);
	m_animDummy[IDLE_DUMMY] = InitAnimation(sf::Vector2i(400, 200), 20, 16, 0, true);

	m_animDummyStatus = IDLE_DUMMY;

	m_paternsSprites[BACKGROUND] = LoadSprite("Sprites/Map/Famine/BackgroundTuto.png", false);
	m_paternsSprites[FOREGROUND] = LoadSprite("Sprites/Map/Famine/ForegroundTuto.png", false);

	text = "If you want to move left or right press Q or D";
	m_textString = "";

	m_font.loadFromFile("Font/Chapbook-Regular.otf");

	m_text.setFont(m_font);
	m_text.setString(text);
	m_text.setCharacterSize(30);
	m_text.setFillColor(sf::Color::White);

	m_textPos = sf::Vector2f(World2ScreenVec(_worldPos).x - m_text.getGlobalBounds().width / 2, World2ScreenVec(_worldPos).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	m_stepTuto = MOVE;
	m_leterToDraw = 0;
	m_timerText = 0;
	m_grabed = 0;


}

void BossFamine::Update(float _dt, b2World* _world, CamData* _camData, ParticleData* _particleData, IndicatorData* _indicatorData, SoundData* _soundData)
{

	
	
	Animation(m_paternsSprites[BOSS_SPRITE_FAMINE], m_animDummy, m_animDummyStatus, _dt);

	if (m_stepTuto == MOVE)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_text.setString("");
			m_stepTuto = JUMP;
		}
	}
	else if (m_stepTuto == JUMP)
	{
		std::string text = "If you want jump press SPACE";

		DisplayLetter(_dt, text);

		if (m_leterToDraw >= text.size() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			m_textString = "";
			m_stepTuto = ATTACK;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	}
	else if (m_stepTuto == ATTACK)
	{
		std::string text = "Use the mouse LEFT CLICK to attack";

		DisplayLetter(_dt, text);

		if (m_leterToDraw >= text.size() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_textString = "";
			m_stepTuto = SELECT_POWER;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	}
	else if (m_stepTuto == SELECT_POWER)
	{
		std::string text = "Press X to select a power";

		DisplayLetter(_dt, text);

		if (m_leterToDraw >= text.size() && sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			m_textString = "";
			m_stepTuto = USE_POWER;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	}
	else if (m_stepTuto == USE_POWER)
	{
		std::string text = "Press mouse MIDLE CLICK to use your power";

		DisplayLetter(_dt, text);

		if (m_leterToDraw >= text.size() && sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			m_textString = "";
			m_stepTuto = GRAB;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	}
	else if (m_stepTuto == GRAB)
	{
		std::string text = "Use the mouse RIGHT CLICK to use your grappling";

		DisplayLetter(_dt, text);

		if (m_leterToDraw >= text.size() && sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			m_textString = "";
			m_stepTuto = HUNG;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2 - 30, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	}
	else if (m_stepTuto == HUNG)
	{
		std::string text = "Try to hang on here";

		DisplayLetter(_dt, text);

		if (m_leterToDraw >= text.size() && m_grabed)
		{
			m_textString = "";
			m_stepTuto = BEFOR_FIGHT;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(985 - m_text.getGlobalBounds().width / 2 - 30, 200);
	}
	else if (m_stepTuto == BEFOR_FIGHT)
	{
		std::string text = "Try to fight this dummy, pay attention to his movements !";

		DisplayLetter(_dt, text);

		if (m_timerText > 1.5)
		{
			m_textString = "";
			m_stepTuto = FIGHT;
			m_timerText = 0;
			m_leterToDraw = 0;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2 - 30, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);
	}
	else if (m_stepTuto == FIGHT)
	{
		float distancePlayerBosX = m_playerPos.x - m_body->GetPosition().x;
		std::string currentAttack;

		m_paternsSprites[PROJECTILE].rotate(10);

		m_globalTimer -= _dt;


		switch (m_currentPatern)
		{
		case SUCTION:
			currentAttack = "Suction attack !";
			SuctionPatern(_world);
			break;
		case GROUND_ATTACK:
			currentAttack = "Ground attack !";
			GroundAttackPatern(_world);
			break;
		case STRAIGHT_ARM:
			currentAttack = "Straight attack !";
			StraightPatern(_world);
			break;
		default:
			break;
		}


		DisplayLetter(_dt, currentAttack);

		if (m_globalTimer <= 0)
		{
			m_currentPatern = m_tempShowPatern /*rand() % NB_PATERNS*/;
			m_tempShowPatern++;

			m_textString = "";
			m_timerText = 0;
			m_leterToDraw = 0;
			m_animDummy->timeAnim = 0;

			switch (m_currentPatern)
			{
			case SUCTION:
				m_globalTimer = TIME_SUCTION;
				m_animDummyStatus = SUCTION_DUMMY;
				break;
			case GROUND_ATTACK:
				m_globalTimer = TIME_GROUNDATTACK;
				m_animDummyStatus = GROUND_DUMMY;
				break;
			case STRAIGHT_ARM:
				m_globalTimer = TIME_STRAIGHT_ARM;
				m_animDummyStatus = STRAIGHT_DUMMY;
				m_tempShowPatern = 0;
				break;
			default:
				break;
			}
		}

		if (distancePlayerBosX <= 0)
		{
			m_dir = RIGHT;
		}
		else
		{
			m_dir = LEFT;
		}

		if (m_hp <= 0)
		{
			ClearFight(_world);
			m_currentPatern = END_FIGHT;
		}

		m_text.setString(m_textString);
		m_textPos = sf::Vector2f(World2Screen(m_body->GetPosition()).x - m_text.getGlobalBounds().width / 2, World2Screen(m_body->GetPosition()).y - Meter2Pixel(BOSS_SIZE_Y) - 50);

	}


	m_spriteHelice.rotate(10 * _dt);

	if (m_stepTuto != FIGHT)
	{
		//m_hp = 100;
	}

}

void BossFamine::SuctionInit(b2World* _world)
{
	B2Dinfo suctionInfo;
	suctionInfo.type = b2_kinematicBody;
	suctionInfo.shape = CIRCLE;
	suctionInfo.userData.fixtureName = "Suction";
	suctionInfo.pos = sf::Vector2f(m_body->GetPosition().x + BOSS_SIZE_X / 2, m_body->GetPosition().y + BOSS_SIZE_Y / 2);
	suctionInfo.radius = 0.1;
	suctionInfo.density = 0;
	suctionInfo.friction = 1;
	suctionInfo.fixedRotation = true;

	m_bodySuction = CreateBodyB2D(_world, suctionInfo);
	m_bodySuction->GetFixtureList()[0].SetSensor(true);
}

void BossFamine::SuctionPatern(b2World* _world)
{
	if (m_globalTimer <= TIME_SUCTION - TIME_PREPARE_SUCTION && m_bodySuction == nullptr)
	{
		m_globalTimer -= rand() % 4;
		SuctionInit(_world);
	}
	else if (m_globalTimer <= 0 && m_bodySuction != nullptr)
	{
		_world->DestroyBody(m_bodySuction);
		m_bodySuction = nullptr;
	}
}

bool BossFamine::GroundAttack(b2World* _world)
{
	sf::Vector2f distancePlayerBoss;
	B2Dinfo groundAttackInfo;

	groundAttackInfo.type = b2_kinematicBody;
	groundAttackInfo.shape = CIRCLE;
	groundAttackInfo.userData.fixtureName = "Ground attack";
	groundAttackInfo.radius = 0.5;
	groundAttackInfo.density = 0;
	groundAttackInfo.friction = 1;
	groundAttackInfo.fixedRotation = true;

	distancePlayerBoss.x = m_playerPos.x - m_body->GetPosition().x;
	distancePlayerBoss.y = m_playerPos.y - m_body->GetPosition().y - BOSS_SIZE_Y / 2;

	groundAttackInfo.pos.x = m_playerPos.x + distancePlayerBoss.y * m_dir + (OFFSET_ATTACK_X * m_dir);
	groundAttackInfo.pos.y = m_body->GetPosition().y - BOSS_SIZE_Y / 2;

	m_dirGround = M_PI_4;

	if (m_dir == RIGHT)
	{
		m_dirGround = 3 * M_PI / 4;
	}

	if (abs(distancePlayerBoss.x) >= 4 + abs(distancePlayerBoss.y))
	{
		m_groundAttack = CreateBodyB2D(_world, groundAttackInfo);
		m_groundAttack->SetLinearVelocity(b2Vec2(cos(m_dirGround) * GROUND_PROJECTILE_SPEED, sin(m_dirGround) * GROUND_PROJECTILE_SPEED));
		return false;
	}
	return true;
}

void BossFamine::GroundAttackPatern(b2World* _world)
{
	float frameTimeGroundAttack = 1.f / m_animDummy[GROUND_DUMMY].framerate;
	int frameNbGroundAttack = m_animDummy->timeAnim / frameTimeGroundAttack;

	if (frameNbGroundAttack >= m_animDummy[GROUND_DUMMY].frameNb)
	{
		m_animDummyStatus = IDLE_DUMMY;
	}


	if (m_globalTimer <= TIME_GROUNDATTACK - TIME_PREPARE_GROUNDATTACK && m_groundAttack == nullptr)
	{
		if (GroundAttack(_world))
		{
			m_globalTimer = 0;
		}
	}
	else if (m_globalTimer <= 0 && m_groundAttack != nullptr)
	{
		_world->DestroyBody(m_groundAttack);
		m_groundAttack = nullptr;
	}
}



void BossFamine::StraightPatern(b2World* _world)
{
	float frameTimeStraightAttack = 1.f / m_animDummy[STRAIGHT_DUMMY].framerate;
	int frameNbStraightAttack = m_animDummy->timeAnim / frameTimeStraightAttack;

	if (frameNbStraightAttack >= m_animDummy[STRAIGHT_DUMMY].frameNb)
	{
		m_animDummyStatus = IDLE_DUMMY;
	}

	if (m_globalTimer <= TIME_CHARGE_ARM && m_straightBody == nullptr)
	{
		B2Dinfo straightAttackInfo;

		straightAttackInfo.type = b2_kinematicBody;
		straightAttackInfo.shape = CIRCLE;
		straightAttackInfo.userData.fixtureName = "Straight attack";
		straightAttackInfo.radius = 0.5;
		straightAttackInfo.density = 0;
		straightAttackInfo.friction = 1;
		straightAttackInfo.fixedRotation = true;
		straightAttackInfo.pos = { m_body->GetPosition().x, float(m_body->GetPosition().y + BOSS_SIZE_Y / 2) };

		if (m_playerPos.x > m_body->GetPosition().x)
		{
			m_dirStraightAttack = RIGHT;
			straightAttackInfo.pos.x += BOSS_SIZE_X + straightAttackInfo.radius;
		}
		else
		{
			m_dirStraightAttack = LEFT;
			straightAttackInfo.pos.x -= straightAttackInfo.radius;
		}

		m_straightBody = CreateBodyB2D(_world, straightAttackInfo);
		m_straightBody->SetLinearVelocity(b2Vec2(SPEED_ARM * m_dirStraightAttack, 0));
	}

	if (m_globalTimer <= 0)
	{
		_world->DestroyBody(m_straightBody);
		m_straightBody = nullptr;
	}
}

void BossFamine::ClearFight(b2World* _world)
{
	if (m_straightBody != nullptr)
	{
		_world->DestroyBody(m_straightBody);
		m_straightBody = nullptr;
	}

	if (m_groundAttack != nullptr)
	{
		_world->DestroyBody(m_groundAttack);
		m_groundAttack = nullptr;
	}

	if (m_bodySuction != nullptr)
	{
		_world->DestroyBody(m_bodySuction);
		m_bodySuction = nullptr;
	}
}


void BossFamine::Display(sf::RenderWindow& _window)
{
	sf::Vector2f dummyPos = World2Screen(b2Vec2(m_body->GetPosition().x, m_body->GetPosition().y + BOSS_SIZE_Y));

	BlitSprite(m_paternsSprites[BACKGROUND], sf::Vector2f(0, -300), 0, _window);
	BlitSprite(m_spriteHelice, sf::Vector2f(990 + 250, 0), m_spriteHelice.getRotation(), _window);

	BlitSprite(m_paternsSprites[BOSS_SPRITE_FAMINE], sf::Vector2f(dummyPos.x - 140, dummyPos.y - 25), 0, _window);

	if (m_straightBody != nullptr)
	{
		BlitSprite(m_paternsSprites[PROJECTILE], World2Screen(m_straightBody->GetPosition()), m_paternsSprites[PROJECTILE].getRotation(), _window);
	}

	if (m_groundAttack != nullptr)
	{
		BlitSprite(m_paternsSprites[PROJECTILE], World2Screen(m_groundAttack->GetPosition()), m_paternsSprites[PROJECTILE].getRotation(), _window);
	}


}

void BossFamine::DisplayFront(sf::RenderWindow& _window)
{
	BlitSprite(m_groundFront, sf::Vector2f(0, -300), 0, _window);
	BlitSprite(m_paternsSprites[FOREGROUND], sf::Vector2f(0, -300), 0, _window);
	BlitText(m_text, m_textPos, 0, _window);

}

void BossFamine::DisplayLetter(float _dt, std::string _textToDraw)
{
	m_timerText += _dt;

	if (m_timerText > SPEED_DISPLAY_LETER && m_leterToDraw < _textToDraw.size())
	{

		m_textString += _textToDraw[m_leterToDraw];

		m_timerText = 0;
		m_leterToDraw++;
	}

}