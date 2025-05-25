#include "Collide.h"

void AllCollides(GameData& _gameData, b2World* _world)
{
	std::string testMessage = "";
	bool testInCorrosive = false;

	for (auto message : _gameData.listener->m_collideMessage)
	{
		// player
		PlayerCollide(_gameData, _world, message);
		PandemicCollide(_gameData, _world, message);
		FamineCollide(_gameData, _world, message);

		if (_gameData.currentBoss != nullptr)
		{

			if (message == "BOSS HIT")
			{
				BossData* pointer = (BossData*)_gameData.currentBoss->GetDaughter();


				_gameData.currentBoss->TakeDamage(10);
				

				//Create blood between player atk and boss
				if (_gameData.playerData->GetAttackBody() != nullptr)
				{
					sf::Vector2f posAtk = World2Screen(_gameData.playerData->GetAttackBody()->GetPosition() + b2Vec2(_gameData.playerData->GetAttackSize().x / 2, _gameData.playerData->GetAttackSize().y / 2));

					_gameData.particleData->CreateBloodParticle(posAtk, World2Screen(pointer->GetBody()->GetPosition() - _gameData.playerData->GetAttackBody()->GetPosition()), 1200, 0.4, 5);

					switch (_gameData.playerData->GetDirAttack())
					{
					case LEFT:
						_gameData.particleData->CreateAttackParticle(posAtk, SLASH_LEFT);
						break;
					case RIGHT:
						_gameData.particleData->CreateAttackParticle(posAtk, SLASH_RIGHT);
						break;
					case TOP:
						_gameData.particleData->CreateAttackParticle(posAtk, SLASH_UP);
						break;
					case DOWN:
						_gameData.particleData->CreateAttackParticle(posAtk, SLASH_DOWN);
						break;
					default:
						break;
					}
				}
			}

			if (testMessage == "hook hit cristal")
			{
				std::string string = "Cristal" + message;
				BossDeath* ptr = (BossDeath*)_gameData.currentBoss->GetDaughter();
				b2Body* body = GetBodyFromWorld(string, _world);
				if (body != nullptr)
				{
					ptr->GetCristalData()->CristalExplode(_world, std::stoi(message), true);
					_gameData.camData->ShakeCamera(10, 20, 0.5);
				}
			}
			if (message == "HOOK HIT CRISTAL")
			{
				testMessage = "hook hit cristal";
			}

			if (testMessage == "ghost hit")
			{
				std::string string = "tempdmg" + (message);
				b2Body* body = GetBodyFromWorld(string, _world);
				BossDeath* ptr = (BossDeath*)_gameData.currentBoss->GetDaughter();
				int messageInt;
				if (message != "GHOST HIT")
				{
					messageInt = std::stoi(message.c_str());

					if (body != nullptr && messageInt != (int)"GHOST HIT")
					{
						ptr->DeleteGhostTab(messageInt, _world);
					}
				}
				testMessage = "";
			}

			if (message == "GHOST HIT")
			{
				testMessage = "ghost hit";
			}

			if (message == "COMET COLLIDE")
			{
				b2Body* body = GetBodyFromWorld("Comet", _world);
				BossDeath* ptr = (BossDeath*)_gameData.currentBoss->GetDaughter();
				ptr->CometExploInit(body->GetPosition(), _world);
				_world->DestroyBody(body);

			}
			

			//Bounce balls with wall
			if (testMessage == "bounce wall balls")
			{
				std::string string = "Balls" + message;
				BossDeath* ptr = (BossDeath*)_gameData.currentBoss->GetDaughter();
				std::vector<b2Body*>* ptrVector = ptr->GetBallsVector();
				b2Body* body = GetBodyFromWorld(string, _world);
				if (ptr->GetBallsShouldBounce())
				{
					if (body != nullptr)
					{
						ptrVector->at(std::stoi(message))->SetLinearVelocity(b2Vec2(-body->GetLinearVelocity().x, body->GetLinearVelocity().y));
					}
				}
				testMessage = "";
			}

			if (message == "BOUNCE WALL BALLS")
			{
				testMessage = "bounce wall balls";
			}

			//Bounce balls with ground and roof
			if (testMessage == "bounce y axis balls")
			{
				std::string string = "Balls" + message;
				BossDeath* ptr = (BossDeath*)_gameData.currentBoss->GetDaughter();
				std::vector<b2Body*>* ptrVector = ptr->GetBallsVector();
				b2Body* body = GetBodyFromWorld(string, _world);
				if (ptr->GetBallsShouldBounce())
				{
					if (body != nullptr)
					{
						ptrVector->at(std::stoi(message))->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -body->GetLinearVelocity().y));
					}
				}
				testMessage = "";
			}

			if (message == "BOUNCE Y AXIS BALLS")
			{
				testMessage = "bounce y axis balls";
			}

			if (message == "PLAYER IN SAFEZONE")
			{
				_gameData.playerData->AddValueSafeZoneIn(1);
			}
			if (message == "PLAYER NOT IN SAFEZONE")
			{
				_gameData.playerData->AddValueSafeZoneIn(-1);
			}

		}
	}

	for (auto& body : _gameData.listener->m_allBodyDel)
	{
		std::string bodyName = ((UserData*)body->GetFixtureList()[0].GetUserData().pointer)->fixtureName;

		_world->DestroyBody(body);
	}

	_gameData.listener->m_allBodyDel.clear();
	_gameData.listener->m_collideMessage.clear();
}


void MyContactListener::BeginContact(b2Contact* _contact)
{

	b2Fixture* fixA = _contact->GetFixtureA();
	b2Fixture* fixB = _contact->GetFixtureB();

	b2Body* bodyA = fixA->GetBody();
	b2Body* bodyB = fixB->GetBody();


	if (_contact->GetFixtureA()->GetUserData().pointer != NULL && _contact->GetFixtureB()->GetUserData().pointer != NULL)
	{

		std::string nomA = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->fixtureName;
		std::string nomB = ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->fixtureName;

		PlayerListenerCollide(_contact);
		FamineListenerCollide(_contact);
		PandemicListenerCollide(_contact);

		//Hitbox between grappling hook and cristal
		if (nomA == std::string("Grapling") || nomB == std::string("Grapling"))
		{
			std::string stringB;
			std::string indexB;

			if (nomA == std::string("Grapling"))
			{
				stringB = nomB;
				indexB = nomB;
			}
			else
			{
				stringB = nomA;
				indexB = nomA;
			}

			stringB.erase(stringB.begin() + 7, stringB.end());
			indexB.erase(indexB.begin(), indexB.begin() + 7);
			if (stringB == "Cristal")
			{
				m_collideMessage.push_back("HOOK HIT CRISTAL");
				m_collideMessage.push_back(indexB);
			}
		}

		//Hitbox between safezone and cristal
		if (nomA == std::string("Safezone") || nomB == std::string("Safezone"))
		{
			std::string stringB;
			std::string indexB;

			if (nomA == std::string("Safezone"))
			{
				stringB = nomB;
				indexB = nomB;
			}
			else
			{
				stringB = nomA;
				indexB = nomA;
			}

			stringB.erase(stringB.begin() + 7, stringB.end());
			indexB.erase(indexB.begin(), indexB.begin() + 7);
			if (stringB == "Cristal")
			{
				m_collideMessage.push_back("HOOK HIT CRISTAL");
				m_collideMessage.push_back(indexB);
			}
		}

		//Hitbox between safezone and player
		if ((nomA == std::string("Safezone") && nomB == std::string("Player")) || (nomA == std::string("Player") && nomB == std::string("Safezone")))
		{
			m_collideMessage.push_back("PLAYER IN SAFEZONE");
		}

		//Hitbox between ghost and center
		if (nomA == std::string("center") || nomB == std::string("center"))
		{
			std::string stringB = nomB;
			std::string indexB = nomB;

			if (nomA == std::string("center"))
			{
				stringB = nomB;
				indexB = nomB;
			}
			else
			{
				stringB = nomA;
				indexB = nomA;
			}

			stringB.erase(stringB.begin() + 7, stringB.end());
			indexB.erase(indexB.begin(), indexB.begin() + 7);

			if (stringB == "tempdmg")
			{
				m_collideMessage.push_back("GHOST HIT");
				m_collideMessage.push_back(indexB);
			}
		}

		//Balls bounce
		if (nomA == std::string("Platform") || nomB == std::string("Platform"))
		{
			std::string stringB;
			std::string indexB;

			if (nomA == std::string("Platform"))
			{
				stringB = nomB;
				indexB = nomB;
			}
			else
			{
				stringB = nomA;
				indexB = nomA;
			}

			stringB.erase(stringB.begin() + 5, stringB.end());
			indexB.erase(indexB.begin(), indexB.begin() + 5);
			if (stringB == "Balls")
			{
				m_collideMessage.push_back("BOUNCE Y AXIS BALLS");
				m_collideMessage.push_back(indexB);
			}
		}
		if (nomA == std::string("Wall") || nomB == std::string("Wall"))
		{
			std::string stringB;
			std::string indexB;

			if (nomA == std::string("Wall"))
			{
				stringB = nomB;
				indexB = nomB;
			}
			else
			{
				stringB = nomA;
				indexB = nomA;
			}

			stringB.erase(stringB.begin() + 5, stringB.end());
			indexB.erase(indexB.begin(), indexB.begin() + 5);
			if (stringB == "Balls")
			{
				m_collideMessage.push_back("BOUNCE WALL BALLS");
				m_collideMessage.push_back(indexB);
			}
		}

		if (nomA == std::string("Comet") && nomB == std::string("Platform") || nomA == std::string("Platform") && nomB == std::string("Comet"))
		{
			m_collideMessage.push_back("COMET COLLIDE");
		}
		if (nomA == std::string("Comet") && nomB == std::string("Player") || nomA == std::string("Player") && nomB == std::string("Comet"))
		{
			m_collideMessage.push_back("PLAYER HIT");
		}
		if (nomA == std::string("ExploComet") && nomB == std::string("Player") || nomA == std::string("Player") && nomB == std::string("ExploComet"))
		{
			m_collideMessage.push_back("PLAYER HIT");
		}
	}

}
void MyContactListener::EndContact(b2Contact* _contact)
{
	b2Fixture* fixA = _contact->GetFixtureA();
	b2Fixture* fixB = _contact->GetFixtureB();

	b2Body* bodyA = fixA->GetBody();
	b2Body* bodyB = fixB->GetBody();


	if (_contact->GetFixtureA()->GetUserData().pointer != NULL && _contact->GetFixtureB()->GetUserData().pointer != NULL)
	{
		std::string nomA = *((std::string*)_contact->GetFixtureA()->GetUserData().pointer);
		std::string nomB = *((std::string*)_contact->GetFixtureB()->GetUserData().pointer);

		if (nomA == std::string("Player") && nomB == std::string("Corrosive") || nomA == std::string("Corrosive") && nomB == std::string("Player"))
		{
			m_collideMessage.push_back("NOT SAFE ZONE CORROSIVE");
		}

		//Hitbox between safezone and player
		if ((nomA == std::string("Safezone") && nomB == std::string("Player")) || (nomA == std::string("Player") && nomB == std::string("Safezone")))
		{
			m_collideMessage.push_back("PLAYER NOT IN SAFEZONE");
		}
	}
}


void PlayerCollide(GameData& _gameData, b2World* _world, std::string _message)
{
	if (_message == "HIT")
	{
		switch (_gameData.playerData->GetDirAttack())
		{
		case LEFT:
			_gameData.playerData->SetVelocityX(HITBACK_LEFT);
			break;
		case RIGHT:
			_gameData.playerData->SetVelocityX(-HITBACK_RIGHT);
			break;
		case DOWN:
			_gameData.playerData->SetVelocityY(HITBACK_DOWN);
			break;
		default:
			break;
		}

		if (_gameData.playerData->GetDirAttack() != TOP)
		{
			_gameData.playerData->SetState(HIT_BACK);

		}
	}

	if (_message == "PLAYER HIT")
	{
		//Check if the player can be hitted before
		if (!_gameData.playerData->GetIsInSafeZone())
		{
			switch (_gameData.playerData->GetDir())
			{
			case LEFT:
				_gameData.playerData->TakeDamage(5, BUMP_LEFT, *_gameData.soundData);
				break;
			case RIGHT:
				_gameData.playerData->TakeDamage(5, BUMP_RIGHT, *_gameData.soundData);
				break;
			case DOWN:
				_gameData.playerData->TakeDamage(5, BUMP_LEFT, *_gameData.soundData);
				break;
			default:
				break;
			}
		}
	}

	if (_message == "PLAYER HITED")
	{
		_gameData.playerData->TakeDamage(DAMAGE_FAMINE, _gameData.listener->m_dirBump, *_gameData.soundData);
	}

	if (_message == "CAN JUMP")
	{
		_gameData.playerData->CanJump();
	}

	if (_message == "HOOKED!")
	{
		b2Vec2 hookPos = _gameData.mapData->SearchHook(_gameData.playerData->GetGraplingPos());

		_gameData.playerData->Hooked(hookPos, _gameData.soundData);
		_gameData.playerData->DeleteGrappling(_world);

		_gameData.particleData->CreateHookConnectParticle(World2Screen(hookPos), 0.1);

		if (_gameData.actualyBoss == FAMINE)
		{
			BossFamine* ptrBoss = (BossFamine*)_gameData.currentBoss->GetDaughter();
			ptrBoss->Grabed();
		}
	}

	if (_message == "GRAPLING DON'T HIT")
	{
		_gameData.playerData->DeleteGrappling(_world);
	}

	if (_message == "HUNG")
	{
		_gameData.playerData->Hung();
	}

	if (_message == std::string("PLAYER EXPLOSION"))
	{
		if (rand() % 2)
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_LEFT, *_gameData.soundData);
		}
		else
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_RIGHT, *_gameData.soundData);
		}
	}

	if (_message == std::string("CAN JUMP"))
	{
		_gameData.playerData->CanJump();
	}

	if (_message == "FALL HIT ENNEMY")
	{
		b2Vec2 tempHitBackVec(0, 10);
		_gameData.playerData->GetBody()->SetLinearVelocity(tempHitBackVec);

		_gameData.currentBoss->TakeDamage(FALL_DAMAGE);

		BossData* pointer = (BossData*)_gameData.currentBoss->GetDaughter();

		if (_gameData.playerData->GetFallBody() != nullptr)
		{
			_gameData.particleData->CreateBloodParticle(World2Screen(_gameData.playerData->GetFallBody()->GetPosition() + b2Vec2(_gameData.playerData->GetFallSize().x / 2, _gameData.playerData->GetFallSize().y / 2)), World2Screen(pointer->GetBody()->GetPosition() - _gameData.playerData->GetFallBody()->GetPosition()), 1200, 0.4, 5);
		}


		_gameData.playerData->DestroyFallWrath(_world);
	}

	if (_message == "SHOT HIT ENNEMY")
	{
		_gameData.currentBoss->TakeDamage(SHOT_DAMAGE);

		BossData* pointer = (BossData*)_gameData.currentBoss->GetDaughter();

		if (_gameData.playerData->GetShotBody() != nullptr)
		{
			_gameData.particleData->CreateBloodParticle(World2Screen(_gameData.playerData->GetShotBody()->GetPosition() + b2Vec2(_gameData.playerData->GetShotSize().x / 2, _gameData.playerData->GetShotSize().y / 2)), World2Screen(pointer->GetBody()->GetPosition() - _gameData.playerData->GetShotBody()->GetPosition()), 1200, 0.4, 5);
		}

		_gameData.playerData->DestroyShotSloth(_world);
	}

	if (_message == "FALL HIT SCARECROW")
	{
		b2Vec2 tempHitBackVec(0, 10);
		BossData* pointer = (BossData*)_gameData.currentBoss->GetDaughter();

		if (_gameData.playerData->GetFallBody() != nullptr)
		{
			_gameData.particleData->CreateWheatParticle(World2Screen(_gameData.playerData->GetFallBody()->GetPosition() + b2Vec2(_gameData.playerData->GetFallSize().x / 2, _gameData.playerData->GetFallSize().y / 2)), World2Screen(pointer->GetBody()->GetPosition() - _gameData.playerData->GetFallBody()->GetPosition()), 1200, 0.4, 5);
		}

		_gameData.playerData->DestroyFallWrath(_world);
	}

	if (_message == "SHOT HIT SCARECROW")
	{
		BossData* pointer = (BossData*)_gameData.currentBoss->GetDaughter();

		if (_gameData.playerData->GetShotBody() != nullptr)
		{
			_gameData.particleData->CreateWheatParticle(World2Screen(_gameData.playerData->GetShotBody()->GetPosition() + b2Vec2(_gameData.playerData->GetShotSize().x / 2, _gameData.playerData->GetShotSize().y / 2)), World2Screen(pointer->GetBody()->GetPosition() - _gameData.playerData->GetShotBody()->GetPosition()), 1200, 0.4, 5);
		}

		_gameData.playerData->DestroyShotSloth(_world);
	}

	if (_message == "SHOT HIT WALL")
	{
		_gameData.playerData->DestroyShotSloth(_world);
	}
}
void MyContactListener::PlayerListenerCollide(b2Contact* _contact)
{
	std::string nomA = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->fixtureName;
	std::string nomB = ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->fixtureName;

	if (nomA == std::string("Attack") && nomB == std::string("Enemy") || nomA == std::string("Enemy") && nomB == std::string("Attack"))
	{
		m_collideMessage.push_back("HIT");
	}

	if (nomA == std::string("FallWRATH") && nomB == std::string("Enemy") || nomA == std::string("Enemy") && nomB == std::string("FallWRATH"))
	{
		m_collideMessage.push_back("FALL HIT ENNEMY");
	}

	if (nomA == std::string("ShotSLOTH") && nomB == std::string("Enemy") || nomA == std::string("Enemy") && nomB == std::string("ShotSLOTH"))
	{
		m_collideMessage.push_back("SHOT HIT ENNEMY");
	}

	if (nomA == std::string("FallWRATH") && nomB == std::string("FamineBoss") || nomA == std::string("FamineBoss") && nomB == std::string("FallWRATH"))
	{
		m_collideMessage.push_back("FALL HIT SCARECROW");
	}

	if (nomA == std::string("ShotSLOTH") && nomB == std::string("FamineBoss") || nomA == std::string("FamineBoss") && nomB == std::string("ShotSLOTH"))
	{
		m_collideMessage.push_back("SHOT HIT SCARECROW");
	}

	if ((nomA == std::string("ShotSLOTH") && nomB == std::string("Platform")) || (nomA == std::string("Platform") && nomB == std::string("ShotSLOTH"))
		|| (nomA == std::string("ShotSLOTH") && nomB == std::string("Wall")) || (nomA == std::string("Wall") && nomB == std::string("ShotSLOTH")))
	{
		m_collideMessage.push_back("SHOT HIT WALL");
	}

	if (nomA == std::string("Platform") && nomB == std::string("FootPlayer") || nomA == std::string("FootPlayer") && nomB == std::string("Platform"))
	{
		m_collideMessage.push_back("CAN JUMP");
	}

	if (nomA == std::string("Player") && nomB == std::string("Hook") || nomA == std::string("Hook") && nomB == std::string("Player"))
	{
		m_collideMessage.push_back("HUNG");
	}

	if (nomB == std::string("Grapling") && nomA != std::string("Player") && nomA != std::string("FootPlayer"))
	{
		bool noDuplicata = true;
		bool interruptGrapin = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->interruptGrapin;

		for (auto& element : m_collideMessage)
		{
			if (element == "HOOKED!")
			{
				noDuplicata = false;
			}
		}

		if (nomA == std::string("Hook") && noDuplicata)
		{
			m_collideMessage.push_back("HOOKED!");

		}
		else if (interruptGrapin)
		{
			m_collideMessage.push_back("GRAPLING DON'T HIT");
		}
	}


	if (nomA == std::string("Player") && nomB == std::string("dmg") || nomA == std::string("dmg") && nomB == std::string("Player"))
	{
		m_collideMessage.push_back("PLAYER HIT");
	}

	if (nomA == std::string("Player") && nomB == std::string("Enemy") || nomA == std::string("Enemy") && nomB == std::string("Player"))
	{
		m_collideMessage.push_back("PLAYER HIT");
	}

	if (nomA == std::string("Attack") && nomB == std::string("Enemy") || nomA == std::string("Enemy") && nomB == std::string("Attack"))
	{
		m_collideMessage.push_back("BOSS HIT");

	}
	// ALL PLAYER COLLIDE WITH BOSS PATERN
	if (nomA == std::string("Player"))
	{
		std::string stringB = nomB;
		std::string indexB = nomB;

		stringB.erase(stringB.begin() + 7, stringB.end());
		indexB.erase(indexB.begin(), indexB.begin() + 7);

		if (stringB == "tempdmg")
		{
			m_collideMessage.push_back("HIT");

			m_collideMessage.push_back("TEMPDMG HIT");
			m_collideMessage.push_back(indexB);
			m_collideMessage.push_back("PLAYER HIT");
		}
	}

	if (nomA == std::string("Player"))
	{
		std::string stringB = nomB;
		std::string indexB = nomB;

		stringB.erase(stringB.begin() + 9, stringB.end());
		indexB.erase(indexB.begin(), indexB.begin() + 9);

		if (stringB == "transfert")
		{
			m_collideMessage.push_back("PLAYER HIT");
		}
	}
	if (nomA == std::string("Player"))
	{
		std::string stringB = nomB;
		std::string indexB = nomB;

		if (stringB == "Spike")
		{
			m_collideMessage.push_back("PLAYER HIT");
		}
	}

	if ((nomA == std::string("Player") && nomB == std::string("Projectile")) || (nomA == std::string("Projectile") && nomB == std::string("Player")))
	{
		m_collideMessage.push_back("PLAYER HIT");
	}

	if (nomA == std::string("Player") || nomB == std::string("Player"))
	{
		std::string stringB = nomB;

		if (nomA == std::string("Player"))
		{
			stringB = nomB;
		}
		else
		{
			stringB = nomA;
		}

		stringB.erase(stringB.begin() + 5, stringB.end());
		if (stringB == "Balls")
		{
			m_collideMessage.push_back("PLAYER HIT");
		}
	}
	//end

}

void MyContactListener::PandemicListenerCollide(b2Contact* _contact)
{
	std::string nomA = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->fixtureName;
	std::string nomB = ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->fixtureName;


	if ((nomA == "Slash left" || nomA == "Slash right") && nomB == "Wall")
	{
		std::string nomBParOfMap = ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->partOfMap;

		if (nomBParOfMap == "StartBoss Wall" || nomBParOfMap == "Right Wall")
		{
			m_allBodyDel.push_back(_contact->GetFixtureA()->GetBody());

			if (nomA == "Slash left")
			{
				m_collideMessage.push_back("SLASH LEFT DESTROY");
			}
			else
			{
				m_collideMessage.push_back("SLASH RIGHT DESTROY");
			}
		}

	}
	else if ((nomB == "Slash left" || nomB == "Slash right") && nomA == "Wall")
	{
		std::string nomAParOfMap = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->partOfMap;

		if (nomAParOfMap == "StartBoss Wall" || nomAParOfMap == "Right Wall")
		{
			m_allBodyDel.push_back(_contact->GetFixtureB()->GetBody());

			if (nomB == "Slash left")
			{
				m_collideMessage.push_back("SLASH LEFT DESTROY");
			}
			else
			{
				m_collideMessage.push_back("SLASH RIGHT DESTROY");
			}

		}

	}


	if (nomA == "Enemy" && nomB == "Platform")
	{
		std::string nomBParOfMap = ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->partOfMap;

		if (nomBParOfMap == "Ground")
		{
			m_collideMessage.push_back("HIT GROUND");
		}
	}
	else if (nomB == "Enemy" && nomA == "Platform")
	{
		std::string nomAParOfMap = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->partOfMap;

		if (nomAParOfMap == "Ground")
		{
			m_collideMessage.push_back("HIT GROUND");
		}
	}

	if (nomA == "Rat" && nomB != "Hook" && nomB != "Enemy")
	{
		std::vector<RatShoot>* ratShoot = (std::vector<RatShoot>*) ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->m_ratShootVec;

		for (int i = 0; i < ratShoot->size(); i++)
		{
			if ((*ratShoot)[i].body == _contact->GetFixtureA()->GetBody())
			{
				m_allBodyDel.push_back((*ratShoot)[i].body);
				ratShoot->erase(ratShoot->begin() + i);

			}
		}


	}
	else if (nomB == "Rat" && nomA != "Hook" && nomA != "Enemy")
	{
		std::vector<RatShoot>* ratShoot = (std::vector<RatShoot>*) ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->m_ratShootVec;

		for (int i = 0; i < ratShoot->size(); i++)
		{
			if ((*ratShoot)[i].body == _contact->GetFixtureB()->GetBody())
			{
				m_allBodyDel.push_back((*ratShoot)[i].body);
				ratShoot->erase(ratShoot->begin() + i);
			}
		}
	}


	if (nomA == "Rat" && nomB == "Player" || nomB == "Rat" && nomA == "Player")
	{
		m_collideMessage.push_back("PLAYER HITED");
		m_dirBump = BUMP_LEFT;
	}

	if ((nomA == "Slash left" || nomA == "Slash right") && nomB == "Player" || (nomB == "Slash left" || nomB == "Slash right") && nomA == "Player")
	{
		m_collideMessage.push_back("PLAYER HITED");
		m_dirBump = BUMP_LEFT;
	}


	if (nomA == "Player" && nomB == "Pic")
	{
		m_collideMessage.push_back("PLAYER HITED");
		m_dirBump = BUMP_RIGHT;
	}
	else if (nomB == "Pic" && nomA == "Player")
	{
		m_collideMessage.push_back("PLAYER HITED");
		m_dirBump = BUMP_RIGHT;

	}

	if(nomA == "Spear" && nomB == "Player" || nomB == "Spear" && nomA == "Player")
	{
		m_collideMessage.push_back("PLAYER HITED");
		m_dirBump = BUMP_RIGHT;
	}


	if (nomA == "Spear" && nomB == "Rat Ball" || nomB == "Spear" && nomA == "Rat Ball")
	{
		m_collideMessage.push_back("SPEAR HIT");
	}

}
void PandemicCollide(GameData& _gameData, b2World* _world, std::string _message)
{
	if (_message == "HIT GROUND")
	{
		BossPandemicData* daughterTemp = (BossPandemicData*)_gameData.currentBoss->GetDaughter();

		daughterTemp->HitGround(_world, _gameData.camData);
	}

	if (_message == "SLASH LEFT DESTROY")
	{
		BossPandemicData* daughterTemp = (BossPandemicData*)_gameData.currentBoss->GetDaughter();

		daughterTemp->DestroySlash(0);
	}

	if (_message == "SLASH RIGHT DESTROY")
	{
		BossPandemicData* daughterTemp = (BossPandemicData*)_gameData.currentBoss->GetDaughter();

		daughterTemp->DestroySlash(1);
	}

	if (_message == "SPEAR HIT")
	{
		BossPandemicData* daughterTemp = (BossPandemicData*)_gameData.currentBoss->GetDaughter();

		daughterTemp->SpearHit(_world);
	}
}

void FamineCollide(GameData& _gameData, b2World* _world, std::string _message)
{
	if (_message == "FAMINE GET DAMAGE")
	{
		BossFamineSecondPhase* daughterTemp = (BossFamineSecondPhase*)_gameData.currentBoss->GetDaughter();
		sf::Vector2f posAtk = World2Screen(_gameData.playerData->GetAttackBody()->GetPosition() + b2Vec2(_gameData.playerData->GetAttackSize().x / 2, _gameData.playerData->GetAttackSize().y / 2));
		
		BossData* pointer = (BossData*)_gameData.currentBoss->GetDaughter();

		if (daughterTemp->GetCanHit())
		{
			_gameData.currentBoss->TakeDamage(PLAYER_DAMAGE);

			switch (_gameData.playerData->GetDirAttack())
			{
			case LEFT:
				_gameData.particleData->CreateAttackParticle(posAtk, SLASH_LEFT);
				break;
			case RIGHT:
				_gameData.particleData->CreateAttackParticle(posAtk, SLASH_RIGHT);
				break;
			case TOP:
				_gameData.particleData->CreateAttackParticle(posAtk, SLASH_UP);
				break;
			case DOWN:
				_gameData.particleData->CreateAttackParticle(posAtk, SLASH_DOWN);
				break;
			default:
				break;
			}
		}
		
	}

	if (_message == "PLAYER HITED BOSS FAMINE" && _gameData.currentBoss != nullptr)
	{

		b2Vec2 playerPos = _gameData.playerData->GetBody()->GetPosition();
		b2Vec2 bossPos = _gameData.currentBoss->GetBody()->GetPosition();

		if (playerPos.x > bossPos.x)
		{

			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_RIGHT, *_gameData.soundData);
			//_gameData.playerData->EnemyHitRight();
		}
		else
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_LEFT, *_gameData.soundData);
			//_gameData.playerData->EnemyHitLeft();

		}

	}

	if (_message == "PLAYER HITED FROM GROUND ATTACK")
	{
		b2Vec2 playerPos = _gameData.playerData->GetBody()->GetPosition();
		b2Vec2 attackPos = GetBodyFromWorld("Ground attack", _world)->GetPosition();

		if (playerPos.x > attackPos.x)
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_RIGHT, *_gameData.soundData);

		}
		else
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_LEFT, *_gameData.soundData);


		}

	}

	if (_message == "PLAYER HITED FROM STRAIGHT ATTACK")
	{
		b2Vec2 playerPos = _gameData.playerData->GetBody()->GetPosition();
		b2Vec2 attackPos = GetBodyFromWorld("Straight attack", _world)->GetPosition();

		if (playerPos.x > attackPos.x)
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_RIGHT, *_gameData.soundData);

		}
		else
		{
			_gameData.playerData->TakeDamage(DAMAGE_FAMINE, BUMP_LEFT, *_gameData.soundData);
		}

	}

	//Second phase

	//ATTACK TENTACLE

	if (_message == "ATTACK TENTACLE")
	{
		BossFamineSecondPhase* daughterTemp = (BossFamineSecondPhase*)_gameData.currentBoss->GetDaughter();
		daughterTemp->TentacleTakeDamage();
	}

}
void MyContactListener::FamineListenerCollide(b2Contact* _contact)
{
	std::string nomA = ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->fixtureName;
	std::string nomB = ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->fixtureName;

	if (nomA == std::string("Attack") && nomB == std::string("FamineBoss") || nomA == std::string("FamineBoss") && nomB == std::string("Attack"))
	{
		m_collideMessage.push_back("HIT");
		m_collideMessage.push_back("FAMINE GET DAMAGE");
	}

	if (nomA == std::string("Player") && nomB == std::string("FamineBoss") || nomA == std::string("FamineBoss") && nomB == std::string("Player"))
	{
		m_collideMessage.push_back("PLAYER HITED BOSS FAMINE");
	}

	if (nomA == std::string("Player") && nomB == std::string("Ground attack") || nomA == std::string("Ground attack") && nomB == std::string("Player"))
	{
		m_collideMessage.push_back("PLAYER HITED FROM GROUND ATTACK");
	}

	if (nomA == std::string("Player") && nomB == std::string("Straight attack") || nomA == std::string("Straight attack") && nomB == std::string("Player"))
	{
		m_collideMessage.push_back("PLAYER HITED FROM STRAIGHT ATTACK");
	}

	if (nomA == std::string("Player") && nomB == std::string("TORGUE DIT EXPLOSION"))
	{
		m_collideMessage.push_back("PLAYER EXPLOSION");
	}

	// second phase

	if (nomA == std::string("Attack") && nomB == std::string("Tentacle") || nomA == std::string("Tentacle") && nomB == std::string("Attack"))
	{
		m_collideMessage.push_back("HIT");
		m_collideMessage.push_back("ATTACK TENTACLE");
	}


	if (nomA == std::string("Wall") && nomB == std::string("TearsGround"))
	{
		/*	std::vector<b2Body*>* ptrTearsGroundVec = (std::vector<b2Body*>*) ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->tearsGroundVec;
			std::vector<Anim*>* ptrTearsAnim = (std::vector<Anim*>*) ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->animTears;

			for (int i = 0; i < ptrTearsGroundVec->size(); i++)
			{
				if ((*ptrTearsGroundVec)[i] == _contact->GetFixtureB()->GetBody())
				{
					m_allBodyDel.push_back((*ptrTearsGroundVec)[i]);
					ptrTearsGroundVec->erase(ptrTearsGroundVec->begin() + i);
					ptrTearsAnim->erase(ptrTearsAnim->begin() + i);
				}
			}*/

	}

	//Destroy flufekey when we attack them
	if ((nomA == std::string("Flufekey") && nomB == std::string("Attack") || nomA == std::string("Attack") && nomB == std::string("Flufekey"))
		|| (nomA == std::string("Flufekey") && nomB == std::string("FallWRATH") || nomA == std::string("FallWRATH") && nomB == std::string("Flufekey"))
		|| (nomA == std::string("Flufekey") && nomB == std::string("ShotSLOTH") || nomA == std::string("ShotSLOTH") && nomB == std::string("Flufekey")))
	{
		std::vector<Flufekey>* ptrFlufekeyVec;
		std::vector<Anim*>* ptrFlufekeyVecAnim = (std::vector<Anim*>*) ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->animFlufekey;

		if (nomA == std::string("Flufekey"))
		{
			ptrFlufekeyVec = (std::vector<Flufekey>*) ((UserData*)_contact->GetFixtureA()->GetUserData().pointer)->flufekeyVec;

			for (int i = 0; i < ptrFlufekeyVec->size(); i++)
			{
				if ((*ptrFlufekeyVec)[i].body == _contact->GetFixtureA()->GetBody())
				{
					m_allBodyDel.push_back((*ptrFlufekeyVec)[i].body);
					ptrFlufekeyVec->erase(ptrFlufekeyVec->begin() + i);
					ptrFlufekeyVecAnim->erase(ptrFlufekeyVecAnim->begin() + i);

				}
			}
		}
		else if (nomB == std::string("Flufekey"))
		{
			ptrFlufekeyVec = (std::vector<Flufekey>*) ((UserData*)_contact->GetFixtureB()->GetUserData().pointer)->flufekeyVec;

			for (int i = 0; i < ptrFlufekeyVec->size(); i++)
			{

				if ((*ptrFlufekeyVec)[i].body == _contact->GetFixtureB()->GetBody())
				{
					m_allBodyDel.push_back((*ptrFlufekeyVec)[i].body);
					ptrFlufekeyVec->erase(ptrFlufekeyVec->begin() + i);
				}
			}
		}

		m_collideMessage.push_back("HIT");
	}

	//Flufekey hit player
	if (nomA == std::string("Player") && nomB == std::string("Flufekey"))
	{
		b2Body* playerBody = _contact->GetFixtureA()->GetBody();
		b2Body* flufekeyBody = _contact->GetFixtureB()->GetBody();

		if (playerBody->GetPosition().x < flufekeyBody->GetPosition().x)
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_LEFT;
		}
		else
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_RIGHT;
		}
	}

	if (nomA == std::string("Player") && nomB == std::string("Tears"))
	{
		b2Body* playerBody = _contact->GetFixtureA()->GetBody();
		b2Body* tearsBody = _contact->GetFixtureB()->GetBody();

		if (playerBody->GetPosition().x < tearsBody->GetPosition().x)
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_LEFT;
		}
		else
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_RIGHT;
		}
	}

	if (nomA == std::string("Player") && nomB == std::string("TearsGround"))
	{
		b2Body* playerBody = _contact->GetFixtureA()->GetBody();
		b2Body* tearsGroundBody = _contact->GetFixtureB()->GetBody();

		if (playerBody->GetPosition().x < tearsGroundBody->GetPosition().x)
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_LEFT;
		}
		else
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_RIGHT;
		}
	}

	if (nomA == std::string("Player") && nomB == std::string("Mounth"))
	{
		b2Body* playerBody = _contact->GetFixtureA()->GetBody();
		b2Body* mounthBody = _contact->GetFixtureB()->GetBody();

		if (playerBody->GetPosition().x < mounthBody->GetPosition().x)
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_LEFT;
		}
		else
		{
			m_collideMessage.push_back("PLAYER HITED");
			m_dirBump = BUMP_RIGHT;
		}
	}

	if (nomA == std::string("Attack") && nomB == std::string("FamineSecondPhaseBoss") || nomA == std::string("FamineSecondPhaseBoss") && nomB == std::string("Attack"))
	{
		m_collideMessage.push_back("HIT");
		m_collideMessage.push_back("FAMINE GET DAMAGE");
	}
}