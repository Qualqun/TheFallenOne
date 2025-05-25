#include "Main.h"
#include "SFMLDebugDraw.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_NAME, sf::Style::None);

	window.setVerticalSyncEnabled(true);

	float dt;
	b2World* m_world;
	GameData gameData;
	srand(time(NULL));
	//World
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = new b2World(gravity);

	//affichage de debug
	SFMLDebugDraw debugDrawInstance(window);
	debugDrawInstance.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
	m_world->SetDebugDraw(&debugDrawInstance);

	bool showDebug = false;

	sf::Vector2f cam = sf::Vector2f(0, 0);
	InitGame(gameData, m_world);

	while (window.isOpen())
	{
		sf::Event event;
		dt = gameData.clock.restart().asSeconds();

		while (window.pollEvent(event))
		{

			switch (gameData.gameState)
			{
			case MENU:
				gameData.menuData->Event(event, window, gameData);
				break;
			case GAME:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				{
					gameData.gameState = PAUSE;
					//gameData.camData->SetPosition({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 });
				}

				////////////Cheat Trigger////////////
				/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9))
				{
					if (gameData.currentBoss != nullptr)
					{
						m_world->DestroyBody(gameData.currentBoss->GetBody());
						delete gameData.currentBoss;
						gameData.currentBoss = nullptr;
					}
					gameData.mapData->DestroyMap(m_world);
					gameData.mapData->LoadMapDeath(m_world, *gameData.backgroundData, *gameData.soundData);
					gameData.currentBoss = new BossDeath(m_world, sf::Vector2f{ BOSS_DEATH_SPAWN_X, BOSS_DEATH_SPAWN_Y });
					gameData.camData->SetCameraRange({ 0 , 1920 }, { 0, 1080 });
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9))
				{
					gameData.fadingData->StartDefading(1.f);
				}*/

				break;
			case PAUSE:
				gameData.pauseData->Event(event, window, gameData);
				break;
			case DEATH:
				gameData.deathData->Event(event, window, gameData);
				break;
			default:
				break;
			}

			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonPressed:
				gameData.playerData->MouseButtonPressed(m_world, gameData.camData->GetCamera().getCenter(), window);
				break;
			case sf::Event::KeyPressed:

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B))
				{
					showDebug = !showDebug;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1) && gameData.playerData->GraplingExisting())
				{

					gameData.playerData->DeleteGrappling(m_world);

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2) && gameData.actualyBoss == PANDEMIC)
				{
					BossPandemicData* boss = (BossPandemicData*)gameData.currentBoss->GetDaughter();
					boss->ResetBossStatus();
				}


				break;
			default:
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
		{
			gameData.gameState = DEATH;
			gameData.camData->SetPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
		}

		if (!gameData.freezeData->IsFrameFreezed())
		{
			Update(dt, gameData, m_world, window);
		}
		else
		{
			gameData.freezeData->UpdateFreezedFrame(dt);
			gameData.camData->Update(dt, World2Screen(gameData.playerData->GetBody()->GetPosition()));
		}

		//Affichage
		window.clear(sf::Color(108, 93, 130));
		window.setView(gameData.camData->GetCamera());
		debugDrawInstance.cam = cam; //Indication de la position de la caméra au debugDraw

		switch (gameData.gameState)
		{
		case MENU:
			gameData.menuData->Draw(window);
			break;
		case GAME:
		case PAUSE:
			//m_world->DebugDraw(); //Affichage de debug


			gameData.backgroundData->DrawBackground(window, gameData.camData->GetPosition() - sf::Vector2f{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
			gameData.particleData->DrawMiddleground(window);

			gameData.backgroundData->DrawMiddleground(window, gameData.camData->GetPosition() - sf::Vector2f{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

			if (gameData.currentBoss != nullptr)
			{
				gameData.currentBoss->Display(window);
			}


			gameData.mapData->Draw(window);

			gameData.playerData->Display(window);

			gameData.backgroundData->DrawForeground(window, gameData.camData->GetPosition() - sf::Vector2f{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
			gameData.particleData->Draw(window);
			gameData.indicatorData->Draw(window);

			if (gameData.currentBoss != nullptr)
			{
				gameData.currentBoss->DisplayFront(window);
			}

			gameData.playerData->DisplayPower(window);

			gameData.hudData->Draw(window, *gameData.playerData);
			if (showDebug)
			{
				m_world->DebugDraw(); //Affichage de debug
			}

			gameData.textData->Display(window);

			gameData.fadingData->Draw(window);

			if (gameData.gameState == PAUSE)
			{
				gameData.pauseData->Draw(window, gameData);
			}

			window.draw(gameData.m_credit);
			break;
		case DEATH:
			gameData.deathData->Draw(window);

			window.draw(gameData.deathData->m_oversprite);
			break;
		default:
			break;
		}

		window.display();
		//Sleep(1);
	}
}


void InitGame(GameData& _gameData, b2World* _world)
{
	_gameData.soundData = new SoundData();

	_gameData.playerData = new PlayerData(_world);
	_gameData.backgroundData = new BackgroundData();
	_gameData.menuData = new MenuData(*_gameData.soundData);
	_gameData.pauseData = new PauseData();
	_gameData.listener = new MyContactListener();
	_gameData.fadingData = new FadingData();


	_gameData.mapData = new MapData();
	_gameData.camData = new CamData;
	_gameData.hudData = new HudData(*_gameData.playerData);
	_gameData.deathData = new DeathData();
	_gameData.freezeData = new FreezeData();
	_gameData.particleData = new ParticleData();
	_gameData.textData = new Text();
	_gameData.indicatorData = new IndicatorData();

	_gameData.fading = false;
	_gameData.currentBoss = nullptr;

	_gameData.m_credit.setColor(sf::Color(255, 255, 255, 0));

	//_gameData.mapData->LoadMapFamine(_world, *_gameData.backgroundData, *_gameData.soundData);
	//_gameData.camData->SetCameraRange({ 0 , 1920}, { -280 , 800 });
	//_gameData.backgroundData->LoadPandemicBackground();

	//Spawn Pandemie
	/*_gameData.currentBoss = new BossPandemicData(_world, false, sf::Vector2f{ BOSS_PANDEMY_SPAWN_X, BOSS_PANDEMY_SPAWN_Y });
	_gameData.camData->SetCameraRange({ -1920 , 1920 }, { 0 , 1080 });
	_gameData.camData->SetCameraFollowPlayer(true);
	_gameData.actualyBoss = PANDEMIC;*/

	//Spawn Death
	//_gameData.currentBoss = new BossDeath(_world, sf::Vector2f{ BOSS_PANDEMY_SPAWN_X, BOSS_PANDEMY_SPAWN_Y });
	//_gameData.camData->SetCameraRange({ 0 , 1920 }, { 0, 1080 });

	//Spawn Famine
	//_gameData.currentBoss = new BossFamine(_world, sf::Vector2f{ BOSS_PANDEMY_SPAWN_X, BOSS_PANDEMY_SPAWN_Y });
	//_gameData.camData->SetCameraRange({ 0 , 1920 }, { 0 , 1080 });

	// Spawn Jump
	/*_gameData.mapData->LoadMapJump(_world, *_gameData.backgroundData, *_gameData.soundData);
	_gameData.camData->SetCameraRange({ -1920/2 , 1920/2 }, { -7968 , 118});
	_gameData.camData->SetCameraFollowPlayer(true);*/

	_gameData.gameState = MENU;
	_world->SetContactListener(_gameData.listener);


}