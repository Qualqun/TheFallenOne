#ifndef COLLIDE_H
#define COLLIDE_H

#include "Hud.hpp"
#include "Menu.h"
#include "PauseMenu.hpp"
#include "Player.h"
#include "Map.h"
#include "Background.hpp"
#include "Text.hpp"

#include "BossDeath.h"
#include "BossFamine.h"
#include "BossFamineSecondPhase.h"
#include "BossPandemic.h"
#include "BossWar.h"

#include "Sound.hpp"
#include "Camera.hpp"
#include "Fading.hpp"
#include "FreezeFrame.hpp"

#include "Particle.hpp"

#include "DeathState.h"

class HudData;
class MenuData;
class PauseData;
class SoundData;
class DeathData;
class FreezeData;

class MyContactListener : public b2ContactListener
{
public:
    std::vector<std::string> m_collideMessage;
    std::vector<b2Body*> m_allBodyDel;
    int m_dirBump;

    void BeginContact(b2Contact* _contact);
    void EndContact(b2Contact* _contact);

    void FamineListenerCollide(b2Contact* _contact);
    void PandemicListenerCollide(b2Contact* _contact);
    void PlayerListenerCollide(b2Contact* _contact);
};

enum GameState
{
    GAME,
    MENU,
    PAUSE,
    DEATH,
};

struct GameData
{
    HudData* hudData;
    MenuData* menuData;
    PauseData* pauseData;
    PlayerData* playerData;
    MapData* mapData;
    BackgroundData* backgroundData;

    BossData* currentBoss;

    SoundData* soundData;
    CamData* camData;
    Text* textData;

    DeathData* deathData;
    FadingData* fadingData;
    FreezeData* freezeData;

    ParticleData* particleData;
    IndicatorData* indicatorData;

    MyContactListener* listener;
    GameState gameState;

    sf::Clock clock;

    int actualyBoss;
    bool fading;

    //Credit
    float m_timerDied = 3.f;
    float m_fadeAlpha = 0;
    sf::Sprite m_credit = LoadSprite("Sprites/Boss/Death/Credit.png", false);
};

void AllCollides(GameData& _gameData, b2World* _world);
void PlayerCollide(GameData& _gameData, b2World* _world, std::string _message);
void PandemicCollide(GameData& _gameData, b2World* _world, std::string _message);
void FamineCollide(GameData& _gameData, b2World* _world, std::string _message);

#endif