#ifndef BOSSWAR_H
#define BOSSWAR_H

#include "BossData.h"

#define NB_HAND 4

#define BOSS_WIDTH_WAR 100
#define BOSS_HEIGHT_WAR 100

#define HAND_WIDTH 100
#define HAND_HEIGHT 200

enum stateAttackHandWar
{
	HAND_ATTACK,
	HAND_IDLE,
};
enum stateHandType
{
	SWORD,
	CANON,
	CHAIN,
	CHAKRAM,
};
class BossWarData : public BossData
{
private:
	float timer;
	float timerTp;
	sf::Vector2i mousePosRegistered;

	b2Body* handTab[NB_HAND];
public:
	BossWarData(b2World* _world, sf::Vector2f _vec);
	~BossWarData();

	void Update(float _dt, b2World* _world);
	void Display(sf::RenderWindow& _window);
	void* GetDaughter() { return this; }

	//Paterns

	//Getters And Setters for all private variable
	float GetTimer() { return timer; }
	void SetTimer(float _f) { timer = _f; }
	void DecreaseTimer(float _f) { timer -= _f; }
	void InitBodyBoss(sf::Vector2f _vec, b2World* _world);

	void SetMousePosRegistered(sf::Vector2i _pos) { mousePosRegistered = _pos; }
	sf::Vector2i GetMousePosRegistered() { return mousePosRegistered; }
	void ResetMousePosRegistered() { mousePosRegistered = { 0 , 0 }; }

	b2Body* GetBody() { return m_body; }
	void SetBody(b2Body* _body) { m_body = _body; }

	float GetGlobalTimer() { return m_globalTimer; }
	void IncreaseGlobalTimer(float _f) { m_globalTimer += _f; }
	void SetGlobalTimer(float _f) { m_globalTimer = _f; }

	int GetStateAttack() { return m_currentPatern; }
	void SetStateAttack(int _i) { m_currentPatern = _i; }

	sf::Sprite GetSprite() { return m_sprite; }
	void SetSprite(sf::Sprite _sprite) { m_sprite = _sprite; }
	const b2Vec2 GetPosition() { return m_body->GetWorldCenter(); }
};
#endif