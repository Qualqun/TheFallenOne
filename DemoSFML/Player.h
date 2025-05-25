#ifndef PLAYER_H
#define PLAYER_H

#include "Collide.h"
#include "Animation.hpp"
#include "Text.hpp"
#include "Camera.hpp"
#include "Particle.hpp"
#include "Sound.hpp"

#define HITBACK_DOWN 7.5
#define HITBACK_LEFT 10
#define HITBACK_RIGHT 10

#define PLAYER_DAMAGE 20
#define FALL_DAMAGE 15
#define SHOT_DAMAGE 15

#define TIMER_SEGMENT_GREED 0.05f
#define NBR_CHECKPOINT_GREED 40

#define HITBACK_SHOT 20

#define MAX_HP 100

enum Dir
{
	LEFT,
	RIGHT,
	TOP,
	DOWN,
	ENEMY_HIT_RIGHT,
	ENEMY_HIT_LEFT,
	ENEMY_HIT_UP,

};

enum PlayerState
{
	NORMAL,
	HIT_BACK,
	HITED,
	HOOKED,
};

enum DirPlayerBump
{
	BUMP_RIGHT,
	BUMP_LEFT,
	BUMP_UP,
	NO_BUMP
};


enum PlayerAnimation
{
	IDLE_RIGHT,
	IDLE_LEFT,

	RUN_RIGHT,
	RUN_LEFT,

	ATTACK_RIGHT_ONE,
	ATTACK_RIGHT_TWO,
	ATTACK_LEFT_ONE,
	ATTACK_LEFT_TWO,
	ATTACK_TOP,

	ATTACK_AIR_RIGHT,
	ATTACK_AIR_LEFT,
	ATTACK_AIR_TOP,

	ATTACK_BOT,

	JUMP_RIGHT,
	JUMP_LEFT,

	TRANSITION_RIGHT,
	TRANSITION_LEFT,

	FALL_RIGHT,
	FALL_LEFT,

	FALL_POWER,

	TOTAL_PLAYER_ANIM
};
enum EvntActionsPlayer
{
	NO_ACTIONS,
	ATTACK,
	GRAPLING
};

enum InstantPower
{
	IP_WRATH,
	IP_GREED,
	IP_PRIDE,
	IP_SLOTH,
	IP_ENVY,
	IP_GLUTTONY,
	IP_LUST,
	NBR_POWER,
};

struct PowerData
{
	InstantPower m_enum;
	std::string m_name;
	float m_timer;
	float m_cooldown;
	sf::Color m_color;
	std::vector<b2Body*> m_projTab;
};

struct GameData;

class PlayerData
{
public:
	//PlayerData() {};
	PlayerData(b2World* _m_world);
	~PlayerData() {};

	void MouseButtonPressed(b2World* _world, sf::Vector2f _posCameraCenter, sf::RenderWindow& _window);

	void Update(GameData& _gameData, float _dt, b2World* _world, sf::RenderWindow& _window);
	void UpdateDirection();
	void UpdateEvntActionsPlayer(b2World* _world, SoundData& _sound);

	void Display(sf::RenderWindow& _window);
	void DisplayPower(sf::RenderWindow& _window);
	void Hit(float _dt);
	void Hited(float _dt);
	void Move(float _dt, ParticleData& _particle);
	void Power(GameData& _gameData, sf::RenderWindow& _window, b2World* _world);

	// Grapling functions
	bool GraplingExisting() { return m_graplingBody != nullptr; }
	b2Vec2 GetGraplingPos();
	void HookedUpdate(float _dt);
	void Hooked(b2Vec2 _pos, SoundData* _sound);
	void Hung() { m_hung = true; }
	void DeleteGrappling(b2World* _m_world);

	void CanJump();
	void Sucked(b2Vec2 _posBoss);
	void TakeDamage(int _nbDamage, int _dirBump, SoundData& _sound);

	void SetVelocityX(float _velocityX) { m_velocity.x = _velocityX; }
	void SetVelocityY(float _velocityY) { m_velocity.y = _velocityY; }
	void SetVelocity(b2Vec2 _velocity) { m_velocity = _velocity; }
	void SetState(PlayerState _state);

	void EnemyHitRight();
	void EnemyHitLeft();
	void EnemyHitUp();

	int GetHp() { return m_hp; }
	b2Body* GetBody() { return m_playerBody; }
	b2Body* GetAttackBody() { return m_attackBody; }
	sf::Vector2f GetAttackSize() { return m_sizeAttack; }

	Dir GetDir() { return m_moveDir; }
	int GetDirAttack() { return m_dirAttack; }

	//Instant
	void CreatePowerData(InstantPower _enum, float _cooldown, std::string _name, sf::Color _color);
	void AddPowerData(PowerData _addedPower) { m_powerData.push_back(_addedPower); }
	std::vector<PowerData> GetPowerData() { return m_powerData; }
	int GetSelectedPower() { return m_selectedPower; }
	void SetSelectedPower(int _selectedPower) { m_selectedPower = _selectedPower; }
	std::vector<sf::Sprite> GetSpritePower() { return m_powerSpriteVec; }
	void DestroyFallWrath(b2World* _world) { _world->DestroyBody(m_attackFallBody); m_attackFallBody = nullptr; }
	void DestroyShotSloth(b2World* _world) { _world->DestroyBody(m_shotBodySloth); m_shotBodySloth = nullptr; }
	TextInfo GetTextInfo();
	b2Body* GetShotBody() { return m_shotBodySloth; }
	sf::Vector2f GetShotSize() { return m_sizeAttackShot; }
	b2Body* GetFallBody() { return m_attackFallBody; }
	sf::Vector2f GetFallSize() { return m_sizeAttackFall; }

	bool GetIsInSafeZone() { return m_nbSafeZoneIn > 0 ? true : false; }
	void SetIsInSafeZone(bool _isInSafeZone) { m_isInSafeZone = _isInSafeZone; }
	void AddValueSafeZoneIn(int _safezoneEntered) { m_nbSafeZoneIn += _safezoneEntered; }



	friend class HudData;
private:

	float timerAntiBugFly = 1.f;


	bool m_animTest;
	bool m_dirAnim;
	//player
	//Anim
	sf::Sprite m_spriteSheet;
	Anim* m_anim;
	PlayerAnimation m_stateAnimation;
	//end

	sf::Vector2f m_spriteOffSett;
	sf::Vector2f m_sizeBody; // size of body (on meters)
	b2Vec2 m_velocity; // Velocity of player which will be applied to the body
	PlayerState m_state; //differents state of player
	Dir m_moveDir; // In which direction the player is moving
	bool m_jump; // To do only one jump
	b2Body* m_playerBody;
	int m_hp;

	float m_timerInvicibility;
	float m_timerHitedEnemy;
	float m_timerStep;

	//To more easily implement controller commands, an event system has been put in place: when you press a movement key rather than modifying a lot of variables,
	//it will only change one or two (EvntActionsPlayer) for then modify the rest.
	//and since we don't reset m_evntActionPlayer we use a boolean to prevent the players from doing looping actions
	EvntActionsPlayer m_evntActionPlayer;
	bool m_buttonPressed;

	//attacks
	sf::Vector2f m_sizeAttack; // size of attack (on meters)
	b2Body* m_attackBody;
	float m_attackTimer;
	sf::Vector2f m_attackOffSetDir;
	int m_dirAttack;
	int m_attackStatus;

	//sprites attacks
	int m_secondAttack;
	float m_timeSecondAttack;

	//grapling
	b2Body* m_graplingBody;
	float m_dirGrapling;
	float m_graplingTimer;
	b2Vec2 m_hookedPos;
	bool m_offHooked;
	bool m_hung;

	sf::Sprite m_hookSprite;
	sf::Sprite m_chainSprite;
	sf::Shader m_shaderChain;

	std::vector<std::pair<Anim*, sf::Sprite>> m_animChain;

	//Instant
	int m_selectedPower = 0;
	std::vector<PowerData> m_powerData;
	float m_noMouvement;
	float m_changing;
	float m_clickSwitch;
	float m_noHit;
	float m_noHitAndHeal;
	float m_timerDamage;

	bool m_canSuperFall;
	sf::Vector2f m_sizeAttackFall;
	b2Body* m_attackFallBody;

	float m_timeBetweenRewind;
	std::vector<b2Vec2> m_vecPosForRewind;
	int m_lastCheckPoint;
	bool m_canRewind = false;
	bool m_isRewinding = false;

	bool m_canSuperJump;

	sf::Vector2f m_sizeAttackShot;
	b2Body* m_shotBodySloth;
	sf::Sprite m_shotSprite;

	TextInfo m_textLust;

	std::vector<std::string> m_tabSentencePandemineP1;
	std::vector<std::string> m_tabSentencePandemineP2;
	std::vector<std::string> m_tabSentenceJumpLevel;
	std::vector<std::string> m_tabSentenceReapwarP1;
	std::vector<std::string> m_tabSentenceReapwarP2;
	std::vector<std::string> m_tabSentenceTuto;

	std::vector<sf::Sprite> m_powerSpriteVec;

	sf::Sprite m_wrathPowerSprite;
	sf::Sprite m_greedPowerSprite;
	sf::Sprite m_pridePowerSprite;
	sf::Sprite m_slothPowerSprite;
	sf::Sprite m_envyPowerSprite;
	sf::Sprite m_gluttonyPowerSprite;
	sf::Sprite m_lustPowerSprite;

	sf::Font m_policeLust;		//Déclaration de la police à employer pour le texte
	sf::Text m_text;				//Déclaration d'un texte: permet de gérer la phrase affichée, le positionnement, la couleur..
	// end instant

	bool m_isInSafeZone = false;
	int m_nbSafeZoneIn = 0;
};



#endif