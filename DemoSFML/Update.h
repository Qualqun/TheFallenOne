#ifndef UPDATE_H
#define UPDATE_H

#include "Collide.h"

class GameData;
class PlayerData;

void Update(float _dt, GameData& _gameData, b2World* _world, sf::RenderWindow& _window);

void PlayerAttracted(b2World* _world, PlayerData* _player);
void ChangeBoss(GameData& _gameData, b2World* _world, int _bossChange);
void SpawnFamine(GameData& _gameData, b2World* _world);
void SpawnPandemic(GameData& _gameData, b2World* _world);
void SpawnDeath(GameData& _gameData, b2World* _world);
void SpawnJump(GameData& _gameData, b2World* _world);
#endif