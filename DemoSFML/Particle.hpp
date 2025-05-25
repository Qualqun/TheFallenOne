#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Common.h"
#include "Animation.hpp"

#define GRAVITY 981.f

#define COOLDOWN_SPAWN_LEAF 0.025f
#define COOLDOWN_SPAWN_WALK_DUST 0.01f

enum ParticleType
{
	BLOOD1,
	BLOOD2,
	BLOOD3,
	BLOOD4,
	WHEAT,
	HOOK_CONNECT,
	HOOK_CIRCLE,
	LEAF1,
	LEAF2,
	LEAF3,
	WALK_DUST,
	POWER,
	BOSS_SCREAM,
	DUST_DEATH,
	STAR_SUCKED,
	SLASH_LEFT,
	SLASH_RIGHT,
	SLASH_UP,
	SLASH_DOWN,
	TOTAL_PARTICLE_TYPE
};

//Particle base container
class Particle
{
private:

protected:

	float m_lifespan;
	float m_lifeTime;

	sf::Vector2f m_pos;
	sf::Vector2f m_speed;
	float m_angle;

	ParticleType m_type;

public:
	Particle(sf::Vector2f _pos, float _lifespan);
	~Particle();

	virtual void Update(float _dt) = 0;
	virtual void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite);

	float GetLifespan() { return m_lifespan; }
	float GetLifeTime() { return m_lifeTime; }
	sf::Vector2f GetPos() { return m_pos; }
	float GetAngle() { return m_angle; }
	ParticleType GetType() { return m_type; }

	void AddLifeTime(float _time) { m_lifeTime += _time; }
};

//Particle manager
class ParticleData
{
private:

	std::vector<Particle*> m_particleTab;
	std::vector<Particle*> m_particleTabMiddleground;

	float m_cooldown[TOTAL_PARTICLE_TYPE] = { 0 };

protected:

	sf::Sprite m_sprite[TOTAL_PARTICLE_TYPE];
	sf::Shader m_shader[TOTAL_PARTICLE_TYPE];
	Anim m_anim[TOTAL_PARTICLE_TYPE];

public:
	ParticleData();
	~ParticleData();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window);
	void DrawMiddleground(sf::RenderWindow& _window);


	void CreateParticle(ParticleType _type, unsigned int _amount);

	void ClearAllParticle();

	void AddParticle(Particle& _particle);
	void AddParticleMiddleground(Particle& _particle);

	//Particle creator
	void CreateBloodParticle(sf::Vector2f _pos, sf::Vector2f _dir, float _maxSpeed, float _lifespan, unsigned int _nbParticle);
	void CreateWheatParticle(sf::Vector2f _pos, sf::Vector2f _dir, float _maxSpeed, float _lifespan, unsigned int _nbParticle);
	void CreateHookConnectParticle(sf::Vector2f _pos, float _lifespan);
	void CreateLeafParticle(sf::Vector2f _pos, float _lifespan, unsigned int _nbParticle, bool _skipCooldown, bool _canSpawnInBackground, float _cooldown);
	void CreateWalkDustParticle(sf::Vector2f _pos, int _dir, sf::Color _color, float _lifespan, unsigned int _nbParticle, bool _skipCooldown, float _cooldown);
	void CreatePowerParticle(sf::Vector2f _pos, float _lifespan, sf::Color _color, unsigned int _nbParticle);
	void CreateBossScreamParticle(sf::Vector2f _pos, float _lifespan, float _cooldown);
	void CreateDustDeathParticle(sf::Vector2f _pos, float _maxSpeed, float _lifespan, float _cooldown);
	void CreateStarSuckedParticle(sf::Vector2f _pos, sf::Vector2f _posToReach, float _maxSpeed, float _lifespan, float _cooldown);
	void CreateAttackParticle(sf::Vector2f _pos, ParticleType _slashDir);
};


//Particle type

class BloodParticle : public Particle
{
private:

public:
	BloodParticle(sf::Vector2f _pos, sf::Vector2f _vectorDir, int _maxSpeed, float _lifespan);
	~BloodParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class WheatParticle : public Particle
{
private:

public:
	WheatParticle(sf::Vector2f _pos, sf::Vector2f _vectorDir, int _maxSpeed, float _lifespan);
	~WheatParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class HookConnectParticle : public Particle
{
private:

public:
	HookConnectParticle(sf::Vector2f _pos, float _lifespan, float _angle);
	~HookConnectParticle();

	void Update(float _dt);
};

class HookCircleParticle : public Particle
{
private:
	sf::IntRect m_spriteFrame;
	sf::Sprite* m_ptrSprite;
	Anim m_anim;
public:
	HookCircleParticle(sf::Vector2f _pos, float _lifespan, sf::Sprite& _ptrSprite, Anim& _anim);
	~HookCircleParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class LeafParticle : public Particle
{
private:

public:
	LeafParticle(sf::Vector2f _pos, float _lifespan);
	~LeafParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class WalkDustParticle : public Particle
{
private:
	sf::Color m_color = sf::Color(255, 255, 255);
public:
	//_direction need to be left(-1) or right(1)
	WalkDustParticle(sf::Vector2f _pos, int _direction, sf::Color _color, float _lifespan);
	~WalkDustParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class PowerParticle : public Particle
{
private:
	sf::Color m_color;
	sf::Sprite* m_ptrSprite;
	Anim m_anim;
	sf::IntRect m_spriteFrame;
public:
	PowerParticle(sf::Vector2f _pos, float _lifespan, sf::Color _color, sf::Sprite& _ptrSprite, Anim& _anim);
	~PowerParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class BossScreamParticle : public Particle
{
private:
	float m_scale = 0.f;
	float m_transparancy = 0.f;
public:
	BossScreamParticle(sf::Vector2f _pos, float _lifespan);
	~BossScreamParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class DustDeathParticle : public Particle
{
private:
	float m_scale = 0.f;
	float m_transparancy = 0.f;
	float m_maxTransparancy = 0.f;
public:
	DustDeathParticle(sf::Vector2f _pos, float _lifespan, int _maxSpeed);
	~DustDeathParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class StarSuckedParticle : public Particle
{
private:
	float m_transparancy = 0.f;
	sf::Vector2f m_posToReach = { 0,0 };
	sf::Vector2f m_posOrigin = { 0,0 };
	sf::Vector2f m_vectorDir = { 0,0 };
	sf::Shader* m_ptrShader = nullptr;
public:
	StarSuckedParticle(sf::Vector2f _pos, sf::Vector2f _posToReach, float _lifespan, int _maxSpeed, sf::Shader& _shader);
	~StarSuckedParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

class AttackParticle : public Particle
{
private:
	sf::IntRect m_spriteFrame;
	sf::Sprite* m_ptrSprite;
	Anim m_anim;
public:
	AttackParticle(sf::Vector2f _pos, float _lifespan, ParticleType _slashDir, sf::Sprite* _ptrSprite, Anim& _anim);
	~AttackParticle();

	void Update(float _dt);
	void Draw(sf::RenderWindow& _window, sf::Sprite& _sprite) override;
};

#endif