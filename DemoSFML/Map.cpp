#include "Map.h"

#define LAYER_POS_X (1920-5760)

MapData::MapData()
{
	m_loading = true;

	m_hookSprite = LoadSprite("Sprites/Map/Hook.png", false);
	m_skullSprite = LoadSprite("Sprites/Boss/skullAnim2.png", false);

	sf::FloatRect hitbox = m_hookSprite.getGlobalBounds();
	m_hookSprite.setOrigin({ hitbox.width / 2,hitbox.height / 10 * 9 });

	m_nbBossMap = SPAWN_FAMINE;
	m_status = SPAWN_FAMINE;
	m_skullAnimBlocked = false;


	m_skullAnim[APPARITION_ANIM] = InitAnimation(sf::Vector2i(418, 340), 9, 14, 0, false);
	m_skullAnim[LOADING_ANIM] = InitAnimation(sf::Vector2i(418, 340), 12, 33, 0, false);
}

b2Vec2 MapData::SearchHook(b2Vec2 _posGrapling)
{
	float closerDistance = 1000;
	b2Vec2 hookPos = { 0,0 };

	for (auto& hook : m_hook)
	{
		float distance = sqrt(pow(hook->GetPosition().x - _posGrapling.x, 2) + pow(hook->GetPosition().y - _posGrapling.y, 2));

		if (closerDistance > distance)
		{
			closerDistance = distance;
			hookPos = hook->GetPosition();
		}
	}

	return hookPos;
}

void MapData::CreateHook(sf::Vector2f _pos, b2World* _world)
{
	B2Dinfo hookInfo;
	hookInfo.type = b2_dynamicBody;
	hookInfo.shape = CIRCLE;
	hookInfo.userData.fixtureName = "Hook";
	hookInfo.pos = _pos;
	hookInfo.radius = 0.5;
	hookInfo.density = 0;
	hookInfo.friction = 1;

	m_hook.push_back(CreateBodyB2D(_world, hookInfo));
	m_hook[m_hook.size() - 1]->GetFixtureList()[0].SetSensor(true);
}


void MapData::CreatePlatform(sf::Vector2f _pos, b2Vec2 _size, b2World* _world, std::string _partOfMap = "Default name")
{
	B2Dinfo groundInfo;
	groundInfo.type = b2_staticBody;
	groundInfo.shape = POLYGON;
	groundInfo.userData.fixtureName = "Platform";
	groundInfo.userData.partOfMap = _partOfMap;
	groundInfo.userData.sizePolygone = { _size.x, _size.y };
	groundInfo.pos = _pos;
	groundInfo.points.push_back({ 0, 0 });
	groundInfo.points.push_back({ _size.x, 0 });
	groundInfo.points.push_back({ _size.x, _size.y });
	groundInfo.points.push_back({ 0, _size.y });
	groundInfo.density = 0;
	groundInfo.friction = 1;

	m_ground.push_back(CreateBodyB2D(_world, groundInfo));
}

void MapData::CreateWall(sf::Vector2f _pos, b2Vec2 _size, b2World* _world, std::string _partOfMap = "Default name")
{
	B2Dinfo groundInfo;
	groundInfo.type = b2_staticBody;
	groundInfo.shape = POLYGON;
	groundInfo.userData.fixtureName = "Wall";
	groundInfo.userData.partOfMap = _partOfMap;
	groundInfo.userData.sizePolygone = { _size.x, _size.y };
	groundInfo.pos = _pos;
	groundInfo.points.push_back({ 0, 0 });
	groundInfo.points.push_back({ _size.x, 0 });
	groundInfo.points.push_back({ _size.x, _size.y });
	groundInfo.points.push_back({ 0, _size.y });
	groundInfo.density = 0;
	groundInfo.friction = 1;

	m_wall.push_back(CreateBodyB2D(_world, groundInfo));
}



void MapData::DestroyMap(b2World* _world)
{
	//destroy platform
	for (auto& platform : m_ground)
	{
		_world->DestroyBody(platform);
	}
	m_ground.clear();

	//destroy hook
	for (auto& hook : m_hook)
	{
		_world->DestroyBody(hook);
	}
	m_hook.clear();

	//destroy wall
	for (auto& wall : m_wall)
	{
		_world->DestroyBody(wall);
	}
	m_wall.clear();
}

bool MapData::CheckDistancePlayerSkull(b2World* _world)
{
	b2Body* playerBody = GetBodyFromWorld((std::string)"FootPlayer", _world);
	sf::Vector2f playerPos = { (float)playerBody->GetPosition().x , (float)playerBody->GetPosition().y };
	b2Body* skullBody = GetBodyFromWorld((std::string)"Skull", _world);
	sf::Vector2f skullPos = { (float)skullBody->GetPosition().x , (float)skullBody->GetPosition().y };
	float distancePlayerSkull = sqrtf((skullPos.x - playerPos.x) * (skullPos.x - playerPos.x) +
		(skullPos.y - playerPos.y) * (skullPos.y - playerPos.y));
	distancePlayerSkull *= 64;
	if (distancePlayerSkull >= SKULL_ACTIVATION_RANGE)
	{
		return false;
	}
	else if (distancePlayerSkull < SKULL_ACTIVATION_RANGE)
	{
		return true;
	}
}

void MapData::Update(b2Body* _player, b2World* _world, float _dt, ParticleData& _particle)
{
	if (m_loading == true)
	{
		_player->SetTransform(m_playerSpawnPos, 0);
		m_loading = false;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		DestroyMap(_world);
	}

	if (m_skull != nullptr)
	{
		if (CheckDistancePlayerSkull(_world))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) || m_skullAnimBlocked == true)
			{
				AnimationSkull(m_skullSprite, m_skullAnim, LOADING_ANIM, _dt);
				if (m_skullAnim[LOADING_ANIM].timeAnim >= 0.424242)
				{
					m_skullAnimBlocked = true;
					//std::cout << "ANIM TRUE" << std::endl;
				}
				if (m_skullAnim[LOADING_ANIM].timeAnim >= 2)
				{
					m_skullIsFinish = true;
					b2Body* skullBody = GetBodyFromWorld((std::string)"Skull", _world);
					skullBody->SetTransform({ 500.f, 500.f }, 0);

				}
			}
			else
			{
				AnimationSkull(m_skullSprite, m_skullAnim, APPARITION_ANIM, _dt);
			}
		}
		else
		{
			if (m_skullAnim->timeAnim >= 0)
			{
				AnimationSkull(m_skullSprite, m_skullAnim, APPARITION_ANIM, -_dt);

			}
		}
	}


	float angleRad = 0.f;
	switch (m_nbBossMap)
	{
	case SPAWN_PANDEMIC:
		_particle.CreateLeafParticle(sf::Vector2f((1920 * 2) - rand() % (1920 * 3), -32), 20, 1, false, true, COOLDOWN_SPAWN_LEAF);
		break;
	case SPAWN_WAR:

		break;
	case SPAWN_FAMINE:

		break;
	case SPAWN_DEATH:
		_particle.CreateDustDeathParticle(sf::Vector2f{ (float)(rand() % 1920), (float)(rand() % 1080) }, 20, 15, 0.075f);

		angleRad = DegToRad(rand() % 360);
		_particle.CreateStarSuckedParticle(sf::Vector2f{ SCREEN_WIDTH / 2 - 350 * cosf(angleRad), SCREEN_HEIGHT / 4 - 350 * sinf(angleRad) }, sf::Vector2f{ SCREEN_WIDTH / 2 + 25, SCREEN_HEIGHT / 4 - 5 }, 100, 3, 0.1);
		break;
	default:
		break;
	}
}

void MapData::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < m_hook.size(); i++)
	{
		BlitSprite(m_hookSprite, World2Screen(m_hook[i]->GetPosition()), 0, _window);
	}

	if (m_skull != nullptr)
	{
		if (m_skullIsFinish != true)
		{
			BlitSprite(m_skullSprite, { m_skull->GetPosition().x * 64.f , -(m_skull->GetPosition().y * 64.f) - 200 }, 0, _window);
		}
	}
}

void MapData::LoadMapFamine(b2World* _world, BackgroundData& _background, SoundData& _sound)
{
	m_playerSpawnPos = { 10,-8 };

	_sound.StartBackgroundMusic(MUSIC_FAMINE);

	CreatePlatform(sf::Vector2f(0, -10.3), b2Vec2(Pixel2Meter(1920), -2), _world, "Ground");
	CreatePlatform(sf::Vector2f(0, -10 + Pixel2Meter(1080)), b2Vec2(Pixel2Meter(1920), -2), _world, "Roof");

	CreateWall(sf::Vector2f(0, -10 + Pixel2Meter(1080)), b2Vec2(-2, -Pixel2Meter(1080)), _world, "Left Wall");
	CreateWall(sf::Vector2f(Pixel2Meter(1920), -10 + Pixel2Meter(1080)), b2Vec2(2, -Pixel2Meter(1080)), _world, "Right Wall");
	CreateHook(sf::Vector2f(Pixel2Meter(1920 / 2), -5), _world);
}

void MapData::LoadSecondMapFamine(b2World* _world, BackgroundData& _background)
{
	// 20.9844  -11.2344
	m_playerSpawnPos = { 21,-5 };

	CreateWall(sf::Vector2f(0, 0), b2Vec2(17, -1.3), _world, "Roof left");
	CreateWall(sf::Vector2f(17, 0), b2Vec2(8, -1.3), _world, "Roof midle");
	CreateWall(sf::Vector2f(25, 0), b2Vec2(17, -1.3), _world, "Roof right");

	// Hook after dead boss

	for (int i = 0; i < 8; i++)
	{
		CreateHook(sf::Vector2f(17.5 + i, 2), _world);
	}

	CreateWall(sf::Vector2f(19.5, -8.1), b2Vec2(3.2, -2), _world, "Top column");

	CreateWall(sf::Vector2f(-2.125, 0), b2Vec2(3.8, -24), _world, "Left wall");
	CreateWall(sf::Vector2f(42, 0), b2Vec2(-1.7, -24), _world, "Right wall");
	CreatePlatform(sf::Vector2f(19.5, -16.55), b2Vec2(3.2, -6), _world, "Down column");
	CreatePlatform(sf::Vector2f(7, -6.7), b2Vec2(28.2, -1.5), _world, "Principal platform");
	CreatePlatform(sf::Vector2f(0, -22.2), b2Vec2(42, -1.3), _world, "Ground");

	CreateHook(sf::Vector2f(21.1, -11.2), _world);
	CreateHook(sf::Vector2f(34.5, -1.8), _world);
	CreateHook(sf::Vector2f(39.2, -7.2), _world);
	CreateHook(sf::Vector2f(31.4, -8.6), _world);
	CreateHook(sf::Vector2f(10.5, -8.5), _world);
	CreateHook(sf::Vector2f(2.7, -7.2), _world);
	CreateHook(sf::Vector2f(7.3, -1.75), _world);

	_background.CleanAllGround();
}

void MapData::LoadMapWar(b2World* _world, BackgroundData& _background)
{
	m_playerSpawnPos = { 0,-8 };

	m_groundTablePos["Lava"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-30, -11), b2Vec2(60, -2), _world);

	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-2.5, -10), b2Vec2(5, -2), _world);

	m_wallTablePos["Left Wall"] = m_wall.size();
	CreateWall(sf::Vector2f(-30, 15), b2Vec2(2, -30), _world);

	m_wallTablePos["Roof"] = m_wall.size();
	CreateWall(sf::Vector2f(-30, 15), b2Vec2(60, -2), _world);

	m_wallTablePos["Right Wall"] = m_wall.size();
	CreateWall(sf::Vector2f(30, 15), b2Vec2(-2, -30), _world);

	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(10, -9), b2Vec2(5, -0.5), _world);
	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-15, -9), b2Vec2(5, -0.5), _world);

	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(2, -1), b2Vec2(5, -0.5), _world);
	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-7, -1), b2Vec2(5, -0.5), _world);

	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(20, -3), b2Vec2(5, -0.5), _world);
	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-25, -3), b2Vec2(5, -0.5), _world);

	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(15, 6), b2Vec2(5, -0.5), _world);
	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-20, 6), b2Vec2(5, -0.5), _world);

	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(5, 8), b2Vec2(5, -0.5), _world);
	m_groundTablePos["Floor"] = m_ground.size();
	CreatePlatform(sf::Vector2f(-10, 8), b2Vec2(5, -0.5), _world);

	CreateHook(sf::Vector2f(0, -6), _world);
	CreateHook(sf::Vector2f(15, -4), _world);
	CreateHook(sf::Vector2f(-15, -4), _world);
	CreateHook(sf::Vector2f(26, 2), _world);
	CreateHook(sf::Vector2f(-26, 2), _world);
	CreateHook(sf::Vector2f(24, 9), _world);
	CreateHook(sf::Vector2f(-24, 9), _world);
	CreateHook(sf::Vector2f(13, 8), _world);
	CreateHook(sf::Vector2f(-13, 8), _world);
	CreateHook(sf::Vector2f(9, 1), _world);
	CreateHook(sf::Vector2f(-9, 1), _world);
	CreateHook(sf::Vector2f(0, 9), _world);
}

void MapData::LoadMapPandemic(b2World* _world, BackgroundData& _background, SoundData& _sound)
{

	m_playerSpawnPos = { -Pixel2Meter(1920) + 1 ,-11 };

	_sound.StartBackgroundMusic(500);
	_sound.UseSound(SOURCE_BOSS_PANDEMIC, WIND_SOUND);

	CreatePlatform(sf::Vector2f(-Pixel2Meter(1920), -Pixel2Meter(965)), b2Vec2(Pixel2Meter(1920 * 2), -2), _world, "Ground");
	CreatePlatform(sf::Vector2f(-Pixel2Meter(1920), 0), b2Vec2(Pixel2Meter(1920 * 2), 2), _world, "Roof");

	CreateWall(sf::Vector2f(-Pixel2Meter(1920), -Pixel2Meter(965)), b2Vec2(-2, Pixel2Meter(1080)), _world, "Left Wall");
	CreateWall(sf::Vector2f(Pixel2Meter(1920), -Pixel2Meter(965)), b2Vec2(2, Pixel2Meter(1080)), _world, "Right Wall");

	CreateWall(sf::Vector2f(-Pixel2Meter(1920 / 4), -Pixel2Meter(965)), b2Vec2(-2, Pixel2Meter(1080)), _world, "StartBoss Wall");
	m_wall[m_wall.size() - 1]->GetFixtureList()[0].SetSensor(true);


	//Pos x : -35 Pos y : 503
	CreateHook(Screen2WorldVec(sf::Vector2f(-35, 450)), _world);
	CreateHook(Screen2WorldVec(sf::Vector2f(800, 300)), _world);
	CreateHook(Screen2WorldVec(sf::Vector2f(1600, 380)), _world);


}

void MapData::LoadMapDeath(b2World* _world, BackgroundData& _background, SoundData& _sound)
{
	m_nbBossMap = SPAWN_DEATH;
	m_status = SPAWN_DEATH;
	m_playerSpawnPos = Screen2World({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4 * 3 });

	_sound.StartBackgroundMusic(MUSIC_DEATH);

	CreatePlatform(Screen2WorldVec({ SCREEN_WIDTH / 2,SCREEN_HEIGHT }) + sf::Vector2f{ -15, -2 }, b2Vec2(30, 4), _world, "Ground");
	CreateWall(Screen2WorldVec({ 0,SCREEN_HEIGHT }) + sf::Vector2f{ -0.5f, 0 }, b2Vec2(1, 20), _world, "Left Wall");
	CreateWall(Screen2WorldVec({ SCREEN_WIDTH,SCREEN_HEIGHT }) + sf::Vector2f{ -0.5, 0 }, b2Vec2(1, 20), _world, "Right Wall");
	CreatePlatform(Screen2WorldVec({ 0, 0 }) + sf::Vector2f{ 0, -0.5f }, b2Vec2(30, 2), _world, "Roof");

	//Hook
	CreateHook({ 572 / 64.f , -500 / 64.f }, _world);
	CreateHook({ 1348 / 64.f , -500 / 64.f }, _world);

	CreateSkull(Screen2WorldVec({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 4 * 3 }) - sf::Vector2f{ 2.5, 0 }, { 2,1 }, _world);

	sf::Shader* blackHoleShader = new sf::Shader;
	blackHoleShader->loadFromFile("Shaders/Map/Death/BlackHole.frag", sf::Shader::Fragment);
	sf::Shader* waterShader = new sf::Shader;
	waterShader->loadFromFile("Shaders/Map/Death/Water.frag", sf::Shader::Fragment);

	//Load background death
	_background.CleanAllGround();
	sf::Sprite tempSprite = LoadSprite("Sprites/Map/Death/Sky.png", true);
	tempSprite.setScale(1.1f, 1.1f);
	_background.AddBackground(tempSprite, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	tempSprite.setScale(1, 1);
	tempSprite = LoadSprite("Sprites/Map/Death/Water.png", true);
	_background.AddBackground(tempSprite, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }, *waterShader);
	tempSprite = LoadSprite("Sprites/Map/Death/Rocks.png", true);
	_background.AddMiddleground(tempSprite, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	tempSprite = LoadSprite("Sprites/Map/Death/BlackHoleResize.png", true);
	_background.AddMiddleground(tempSprite, { SCREEN_WIDTH / 2 + 25, SCREEN_HEIGHT / 4 - 5 }, *blackHoleShader);
	tempSprite = LoadSprite("Sprites/Map/Death/Ground.png", true);
	_background.AddMiddleground(tempSprite, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
	tempSprite = LoadSprite("Sprites/Map/Death/Foreground.png", true);
	_background.AddForeground(tempSprite, { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });	
}

void MapData::LoadMapJump(b2World* _world, BackgroundData& _background, SoundData& _sound)
{
	m_playerSpawnPos = { -0.75,1 };
	m_nbBossMap = SPAWN_JUMP_LEVEL;

	// Room
	CreatePlatform(sf::Vector2f(-15, -2), b2Vec2(30, 1), _world, "Floor");
	CreateWall(sf::Vector2f(-15, -2), b2Vec2(2, 127), _world, "Left Wall");
	CreateWall(sf::Vector2f(15, -2), b2Vec2(-2, 127), _world, "Right Wall");

#pragma region First Part : Parkour
	// First part : Parkour + Dash + Jump
	CreatePlatform(sf::Vector2f(-10, 1), b2Vec2(5, -0.5), _world, "Platform_320");
	CreatePlatform(sf::Vector2f(-2.5, 3), b2Vec2(5, -0.5), _world, "Platform_320");
	CreatePlatform(sf::Vector2f(5, 5), b2Vec2(5, -0.5), _world, "Platform_320");

	// Dash
	CreatePlatform(sf::Vector2f(-10, 7.5), b2Vec2(5, -0.5), _world, "Platform_320");

	// Jump
	CreatePlatform(sf::Vector2f(-2.5, 17), b2Vec2(5, -0.5), _world, "Platform_320");
#pragma endregion

#pragma region Second Part : Hook
	// Second Part : Hook
	CreateHook({ 480 / 64.f , 1568 / 64.f }, _world);
	CreateHook({ 0 / 64.f , 2048 / 64.f }, _world);
	CreateHook({ -480 / 64.f , 2528 / 64.f }, _world);

	// Dash + Jump
	CreatePlatform(sf::Vector2f(5, 39), b2Vec2(5, -0.5), _world, "Platform_320");
	CreatePlatform(sf::Vector2f(-2.5, 46.5), b2Vec2(5, -0.5), _world, "Platform_320");
	CreatePlatform(sf::Vector2f(-10, 54), b2Vec2(5, -0.5), _world, "Platform_320");
	CreatePlatform(sf::Vector2f(-2.5, 61.5), b2Vec2(5, -0.5), _world, "Platform_320");
#pragma endregion

#pragma region Third Part : Choice
	// Third Part : Choice

	// PlatformWall
	CreatePlatform(sf::Vector2f(-6.5, 83), b2Vec2(3.5, -0.5), _world, "Platform_224_left");
	CreatePlatform(sf::Vector2f(3, 83), b2Vec2(3.5, -0.5), _world, "Platform_224_right");
	CreatePlatform(sf::Vector2f(-9.5, 102), b2Vec2(19, -0.5), _world, "Platform_1216_arch");

	// Solo Platform / Wall
	CreateWall(sf::Vector2f(-5, 82.5), b2Vec2(0.5, -14.5), _world, "Wall_928");
	CreateWall(sf::Vector2f(4.5, 82.5), b2Vec2(0.5, -14.5), _world, "Wall_928");
	CreatePlatform(sf::Vector2f(-15, 68.5), b2Vec2(1, -0.5), _world, "Platform_672_Left");
	CreatePlatform(sf::Vector2f(4.5, 68.5), b2Vec2(10.5, -0.5), _world, "Platform_672_Right");
	CreateWall(sf::Vector2f(-9.5, 102), b2Vec2(0.5, -29), _world, "Wall_1856");
	CreateWall(sf::Vector2f(9, 102), b2Vec2(0.5, -29), _world, "Wall_1856");

	//Hook
	CreateHook({ 0 / 64.f , 4336 / 64.f }, _world);
	CreateHook({ 0 / 64.f , 4976 / 64.f }, _world);
	CreateHook({ 0 / 64.f , 5616 / 64.f }, _world);



	// Hook on Both Sides
	// LEFT
	CreateHook({ 720 / 64.f , 4960 / 64.f }, _world);
	CreateHook({ 720 / 64.f , 5600 / 64.f }, _world);
	CreateHook({ 720 / 64.f , 6240 / 64.f }, _world);
	CreateHook({ 720 / 64.f , 6880 / 64.f }, _world);

	// RIGHT
	CreateHook({ -720 / 64.f , 4960 / 64.f }, _world);
	CreateHook({ -720 / 64.f , 5600 / 64.f }, _world);
	CreateHook({ -720 / 64.f , 6240 / 64.f }, _world);
	CreateHook({ -720 / 64.f , 6880 / 64.f }, _world);
#pragma endregion

#pragma region Fourth Part : End
	// Fourth Part : End
	CreatePlatform(sf::Vector2f(-2.5, 116), b2Vec2(5, -0.5), _world, "Platform_320");
	CreateWall(sf::Vector2f(-15, 122), b2Vec2(13, 3), _world, "Roof Left");
	CreateWall(sf::Vector2f(15, 122), b2Vec2(-13, 3), _world, "Roof Right");
#pragma endregion


	sf::Sprite sprite_platform_224_left = LoadSprite("Sprites/Map/Level/Platform_224_left.png", false);
	sf::Sprite sprite_platform_224_right = LoadSprite("Sprites/Map/Level/Platform_224_right.png", false);
	sf::Sprite sprite_platform_320 = LoadSprite("Sprites/Map/Level/Platform_320-32.png", false);
	sf::Sprite sprite_platform_672_left = LoadSprite("Sprites/Map/Level/Platform_672_left.png", false);
	sf::Sprite sprite_platform_672_right = LoadSprite("Sprites/Map/Level/Platform_672_right.png", false);
	sf::Sprite sprite_platform_arch = LoadSprite("Sprites/Map/Level/Arch.png", false);
	sf::Sprite sprite_wall_and_ground = LoadSprite("Sprites/Map/Level/Map_Haute.png", false);
	sf::Sprite sprite_background = LoadSprite("Sprites/Map/Level/Map_Haute_Background.png", false);

	b2Vec2 tempPos = { -15, 125 };
	_background.AddBackground(sprite_background, World2Screen(tempPos));
	_background.AddForeground(sprite_wall_and_ground, World2Screen(tempPos));

	for (int i = 0; i < m_ground.size(); i++)
	{
		std::string tempString = ((UserData*)m_ground[i]->GetFixtureList()[0].GetUserData().pointer)->partOfMap;
		b2Vec2 tempPos = m_ground[i]->GetPosition();

		if (tempString == "Platform_224_left")
		{
			_background.AddBackground(sprite_platform_224_left, World2Screen(tempPos));
		}
		if (tempString == "Platform_224_right")
		{
			_background.AddBackground(sprite_platform_224_right, World2Screen(tempPos));
		}
		else if (tempString == "Platform_320")
		{
			_background.AddBackground(sprite_platform_320, World2Screen(tempPos));
		}
		else if (tempString == "Platform_672_Left")
		{
			_background.AddBackground(sprite_platform_672_left, World2Screen(tempPos));
		}
		else if (tempString == "Platform_672_Right")
		{
			_background.AddBackground(sprite_platform_672_right, World2Screen(tempPos));
		}
		else if (tempString == "Platform_1216_arch")
		{
			_background.AddBackground(sprite_platform_arch, World2Screen(tempPos));
		}
	}
}

void MapData::CreateSkull(sf::Vector2f _pos, b2Vec2 _size, b2World* _world)
{
	B2Dinfo skullInfo;
	skullInfo.type = b2_dynamicBody;
	skullInfo.shape = POLYGON;
	skullInfo.userData.fixtureName = "Skull";
	skullInfo.userData.partOfMap = "Skull";
	skullInfo.pos = _pos;
	skullInfo.points.push_back({ 0, 0 });
	skullInfo.points.push_back({ _size.x, 0 });
	skullInfo.points.push_back({ _size.x, _size.y });
	skullInfo.points.push_back({ 0, _size.y });
	skullInfo.density = 0;
	skullInfo.friction = 1;

	m_skullIsFinish = false;
	m_skullAnimBlocked = false;
	m_skullAnim[LOADING_ANIM].timeAnim = 0.f;
	m_skull = CreateBodyB2D(_world, skullInfo);
	m_skull->GetFixtureList()[0].SetSensor(true);
	//std::cout << "SKULL CREATED" << std::endl;
}