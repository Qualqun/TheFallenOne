#include "Update.h"


void Update(float _dt, GameData& _gameData, b2World* _world, sf::RenderWindow& _window)
{
	b2Body* allBody = _world->GetBodyList();
	sf::Vector2i mousePos;
	b2Vec2 mousePosWorld;
	switch (_gameData.gameState)
	{

	case GAME:
		//Simulation (0.02s d'incrément)
		_world->Step(_dt, 30, 30);

		mousePos = sf::Mouse::getPosition(_window) + sf::Vector2i(_gameData.camData->GetCamera().getCenter().x - SCREEN_WIDTH / 2, _gameData.camData->GetCamera().getCenter().y - SCREEN_HEIGHT / 2);
		mousePosWorld = Screen2World((sf::Vector2f)mousePos);

		//std::cout << "Pos x: " << mousePos.x << " Pos y: " << mousePos.y << std::endl;

		// Different spawn boss
		switch (_gameData.mapData->GetStatus())
		{
		case SPAWN_PANDEMIC:
			SpawnPandemic(_gameData, _world);
			_gameData.actualyBoss = PANDEMIC;
			break;
		case SPAWN_FAMINE:
			SpawnFamine(_gameData, _world);
			_gameData.actualyBoss = FAMINE;
			break;
		case SPAWN_DEATH:
			SpawnDeath(_gameData, _world);
			_gameData.actualyBoss = DEATH_BOSS;
			break;
		case SPAWN_JUMP_LEVEL:
			SpawnJump(_gameData, _world);
			break;
		default:
			break;
		}
		if (_gameData.currentBoss != nullptr)
		{
			if (_gameData.currentBoss->GetSprite()->getColor().b < 255)
			{
				float colorBossGreen = _gameData.currentBoss->GetSprite()->getColor().g + (_dt * 400);
				float colorBossBlue = _gameData.currentBoss->GetSprite()->getColor().b + (_dt * 400);
				if (colorBossBlue > 255 || colorBossGreen > 255)
				{
					colorBossBlue = 255;
					colorBossGreen = 255;
				}
				_gameData.currentBoss->GetSprite()->setColor({ 255 , (sf::Uint8)colorBossGreen , (sf::Uint8)colorBossBlue });

			}
		}
		_gameData.playerData->Update(_gameData, _dt, _world, _window);
		_gameData.backgroundData->Update(_dt, _gameData.camData->GetCamera().getCenter());
		_gameData.camData->Update(_dt, World2Screen(_gameData.playerData->GetBody()->GetPosition(), { 0,0 }));

		if (_gameData.currentBoss != nullptr)
		{
			_gameData.currentBoss->SetTimerDamage(_gameData.currentBoss->GetTimerDamage() - _dt);
			_gameData.currentBoss->SetPlayerPos(_gameData.playerData->GetBody()->GetPosition());
			_gameData.currentBoss->Update(_dt, _world, _gameData.camData, _gameData.particleData, _gameData.indicatorData, _gameData.soundData);

			if (_gameData.currentBoss->GetHp() <= 0)
			{
				switch (_gameData.actualyBoss)
				{

				case FAMINE:
					ChangeBoss(_gameData, _world, PANDEMIC);
					break;
				case FAMINE_SECOND:
					ChangeBoss(_gameData, _world, JUMP_LEVEL);
					break;
				case PANDEMIC:
					ChangeBoss(_gameData, _world, FAMINE_SECOND);
					break;
				case DEATH_BOSS:
					if (_gameData.m_timerDied > 0)
					{
						_gameData.m_timerDied -= _dt;
						_gameData.m_fadeAlpha = 255 * (1 - _gameData.m_timerDied / 3.f);
						_gameData.m_credit.setColor(sf::Color(255, 255, 255, _gameData.m_fadeAlpha));
					}
					break;
				default:
					break;
				}
			}
		}

		if (_gameData.mapData->GetNBBossMap() == SPAWN_JUMP_LEVEL)
		{
			if (_gameData.playerData->GetBody()->GetPosition().y >= 123)
			{
				ChangeBoss(_gameData, _world, DEATH_BOSS);
			}
		}

		_gameData.textData->Init(_gameData.playerData->GetTextInfo());

		_gameData.mapData->Update(_gameData.playerData->GetBody(), _world, _dt, *_gameData.particleData);
		AllCollides(_gameData, _world);

		_gameData.hudData->Update(_dt, *_gameData.camData, *_gameData.currentBoss, *_gameData.playerData);
		_gameData.hudData->SetHealth(_gameData.playerData->GetHp());

		_gameData.fadingData->Update(_dt, *_gameData.camData);

		//Famine patern
		PlayerAttracted(_world, _gameData.playerData);

		if (_gameData.playerData->GetHp() <= 0)
		{
			_gameData.gameState = DEATH;
		}

		_gameData.indicatorData->Update(_dt, *_gameData.currentBoss);

		break;
	case MENU:
		_gameData.menuData->Update(_dt, _window, _gameData);
		break;

	case PAUSE:
		_gameData.pauseData->Update(_dt, _window, _gameData);
		break;
	case DEATH:
		_gameData.deathData->Update(_dt, _window);
		break;
	default:
		break;
	}

	_gameData.particleData->Update(_dt);
	_gameData.textData->Update(*_gameData.camData, _dt);
}

void PlayerAttracted(b2World* _world, PlayerData* _player)
{
	b2Body* allBody = _world->GetBodyList();

	for (int i = 0; i < _world->GetBodyCount(); i++)
	{
		if (allBody != nullptr && &allBody->GetFixtureList()[0] != nullptr)
		{
			if (*((std::string*)allBody->GetFixtureList()[0].GetUserData().pointer) == "Suction")
			{
				_player->Sucked(allBody->GetPosition());
				i = _world->GetBodyCount();
			}
			else
			{
				allBody = allBody->GetNext();
			}
		}
	}
}

void ChangeBoss(GameData& _gameData, b2World* _world, int _bossChange)
{
	if (_gameData.fading == false)
	{
		_gameData.fadingData->StartFading(1);
		_gameData.fading = true;

	}

	if (_gameData.fadingData->GetStatus() && _gameData.fading)
	{
		_gameData.mapData->SetLoading(true);
		_gameData.mapData->DestroyMap(_world);

		if (_bossChange == FAMINE)
		{
			_world->DestroyBody(_gameData.currentBoss->GetBody());

			delete _gameData.currentBoss;
			_gameData.currentBoss = nullptr;


			_gameData.mapData->LoadMapFamine(_world, *_gameData.backgroundData, *_gameData.soundData);
			_gameData.camData->SetCameraRange({ -99999,99999 }, { -400,1000 });


			_gameData.clock.restart();

		}

		if (_bossChange == FAMINE_SECOND)
		{

			_world->DestroyBody(_gameData.currentBoss->GetBody());

			delete _gameData.currentBoss;
			_gameData.currentBoss = nullptr;

			_gameData.camData->SetCameraFollowPlayer(true);
			_gameData.camData->SetCameraRangeY(sf::Vector2f(0, 1480));
			_gameData.camData->SetCameraRangeX(sf::Vector2f(0, 2688));



			_gameData.currentBoss = new BossFamineSecondPhase(sf::Vector2f(21.1, -1), _world);
			_gameData.mapData->LoadSecondMapFamine(_world, *_gameData.backgroundData);
			_gameData.actualyBoss = FAMINE_SECOND;

			_gameData.clock.restart();

		}

		if (_bossChange == PANDEMIC)
		{
			_world->DestroyBody(_gameData.currentBoss->GetBody());

			delete _gameData.currentBoss;
			_gameData.currentBoss = nullptr;

			_gameData.mapData->SetStatus(SPAWN_PANDEMIC);
			_gameData.actualyBoss = PANDEMIC;

			_gameData.clock.restart();
		}

		if (_bossChange == DEATH_BOSS)
		{
			if (_gameData.currentBoss != nullptr)
			{
				_world->DestroyBody(_gameData.currentBoss->GetBody());
			}

			delete _gameData.currentBoss;
			_gameData.currentBoss = nullptr;
			_gameData.mapData->LoadMapDeath(_world, *_gameData.backgroundData, *_gameData.soundData);
			_gameData.camData->SetCameraRange({ 0 , 1920 }, { 0, 1080 });
			_gameData.actualyBoss = DEATHBOSS;

			_gameData.clock.restart();

		}

		if (_bossChange == JUMP_LEVEL)
		{
			_world->DestroyBody(_gameData.currentBoss->GetBody());

			delete _gameData.currentBoss;
			_gameData.currentBoss = nullptr;

			SpawnJump(_gameData, _world);
			_gameData.actualyBoss = JUMP_LEVEL;

			_gameData.clock.restart();
		}



		_gameData.camData->SetCameraFollowPlayer(true);
		_gameData.fadingData->StartDefading(3);
		_gameData.fading = false;
	}

}

void SpawnFamine(GameData& _gameData, b2World* _world)
{
	sf::Vector2f posBoss;

	_gameData.mapData->LoadMapFamine(_world, *_gameData.backgroundData, *_gameData.soundData);
	_gameData.camData->SetCameraRange({ 0 , 1920 }, { -300 , 780 });

	posBoss.x = GetBodyOfMap(_world, "Right Wall")->GetPosition().x - BOSS_SIZE_X - Pixel2Meter(200);
	posBoss.y = GetBodyOfMap(_world, "Ground")->GetPosition().y;

	_gameData.currentBoss = new BossFamine(_world, posBoss);
	_gameData.mapData->SetStatus(NOTHING);
	_gameData.fadingData->StartDefading(3);
}

void SpawnPandemic(GameData& _gameData, b2World* _world)
{
	_gameData.mapData->SetLoading(true);
	_gameData.mapData->LoadMapPandemic(_world, *_gameData.backgroundData, *_gameData.soundData);
	//_gameData.backgroundData->LoadPandemicBackground();

	_gameData.currentBoss = new BossPandemicData();
	_gameData.camData->SetCameraRange({ -1920 , 1920 }, { 0 , 1080 });
	_gameData.camData->SetCameraFollowPlayer(true);
	_gameData.actualyBoss = PANDEMIC;



	_gameData.mapData->SetStatus(NOTHING);
	_gameData.clock.restart();
}

void SpawnDeath(GameData& _gameData, b2World* _world)
{
	_gameData.mapData->LoadMapDeath(_world, *_gameData.backgroundData, *_gameData.soundData);
	_gameData.backgroundData->LoadDeathBackground();

	_gameData.currentBoss = new BossDeath(_world, sf::Vector2f{ BOSS_DEATH_SPAWN_X, BOSS_DEATH_SPAWN_Y });
	_gameData.camData->SetCameraRange({ 0 , 1920 }, { 0, 1080 });
	_gameData.camData->SetCameraFollowPlayer(true);
	_gameData.actualyBoss = DEATH_BOSS;

	_gameData.mapData->SetStatus(NOTHING);
	_gameData.clock.restart();
}

void SpawnJump(GameData& _gameData, b2World* _world)
{
	_gameData.mapData->LoadMapJump(_world, *_gameData.backgroundData, *_gameData.soundData);
	_gameData.backgroundData->LoadJumpBackground();

	_gameData.camData->SetCameraRange({ -1920 / 2 , 1920 / 2 }, { -7968 , 118 });
	_gameData.camData->SetCameraFollowPlayer(true);

	_gameData.mapData->SetStatus(NOTHING);
	_gameData.clock.restart();
}