#include "Particle.hpp"

Particle::Particle(sf::Vector2f _pos = { 0,0 }, float _lifespan = 0)
{
	m_lifespan = _lifespan;
	m_lifeTime = 0;

	m_pos = _pos;
	m_speed = { 0,0 };

	m_angle = 0;

	m_type = TOTAL_PARTICLE_TYPE;
}

Particle::~Particle()
{
	m_pos = { NULL,NULL };
	m_lifespan = NULL;
	m_lifeTime = NULL;
	m_speed = { NULL,NULL };
	m_angle = NULL;
	m_type = (ParticleType)NULL;
}

void Particle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setPosition(m_pos);
	_sprite.setRotation(m_angle);

	_window.draw(_sprite);
}

#pragma region Particle manager
ParticleData::ParticleData()
{
	//Load sprite
	m_sprite[BLOOD1] = LoadSprite("Sprites/Particle/Blood.png", true);
	m_sprite[BLOOD2] = LoadSprite("Sprites/Particle/BloodSmall.png", true);
	m_sprite[BLOOD3] = LoadSprite("Sprites/Particle/BloodSpiral.png", true);
	m_sprite[BLOOD4] = LoadSprite("Sprites/Particle/BloodSpiralFlip.png", true);
	m_sprite[WHEAT] = LoadSprite("Sprites/Particle/Wheat.png", true);
	m_sprite[HOOK_CONNECT] = LoadSprite("Sprites/Particle/HookConnect.png", true);
	m_sprite[HOOK_CIRCLE] = LoadSprite("Sprites/Particle/HookCircleSpriteSheet.png", false);
	m_sprite[LEAF1] = LoadSprite("Sprites/Particle/Leaf1.png", true);
	m_sprite[LEAF2] = LoadSprite("Sprites/Particle/Leaf2.png", true);
	m_sprite[LEAF3] = LoadSprite("Sprites/Particle/Leaf3.png", true);
	m_sprite[WALK_DUST] = LoadSprite("Sprites/Particle/WalkDust.png", true);
	m_sprite[POWER] = LoadSprite("Sprites/Particle/Power.png", true);
	m_sprite[POWER].setScale(0.5, 0.5);
	m_sprite[BOSS_SCREAM] = LoadSprite("Sprites/Particle/BossScream.png", true);
	m_sprite[DUST_DEATH] = LoadSprite("Sprites/Particle/DustDeath.png", true);
	m_sprite[STAR_SUCKED] = LoadSprite("Sprites/Particle/Star.png", true);
	m_sprite[SLASH_LEFT] = LoadSprite("Sprites/Particle/SlashLeftSpriteSheet.png", false);
	m_sprite[SLASH_RIGHT] = LoadSprite("Sprites/Particle/SlashRightSpriteSheet.png", false);
	m_sprite[SLASH_UP] = LoadSprite("Sprites/Particle/SlashUpSpriteSheet.png", false);
	m_sprite[SLASH_DOWN] = LoadSprite("Sprites/Particle/SlashDownSpriteSheet.png", false);

	//Load shader
	m_shader[STAR_SUCKED].loadFromFile("Shaders/Map/Death/Streching.frag", sf::Shader::Fragment);

	//Load animation
	m_anim[HOOK_CIRCLE] = InitAnimation(sf::Vector2i(196, 176), 60, 9, 0, false);

	m_anim[SLASH_LEFT] = InitAnimation(sf::Vector2i(633, 534), 15, 5, 0, false);
	m_anim[SLASH_RIGHT] = InitAnimation(sf::Vector2i(633, 534), 15, 5, 0, false);
	m_anim[SLASH_UP] = InitAnimation(sf::Vector2i(238, 511), 15, 5, 0, false);
	m_anim[SLASH_DOWN] = InitAnimation(sf::Vector2i(238, 511), 15, 5, 0, false);

	m_anim[POWER] = InitAnimation(sf::Vector2i(256, 256), 12, 7, 0, false);

	//Set origin of a frame in the spritesheet
	m_sprite[HOOK_CIRCLE].setOrigin({ 196 / 2.f,176 / 2.f });

	m_sprite[SLASH_LEFT].setOrigin({ 633 / 2.f,534 / 2.f });
	m_sprite[SLASH_RIGHT].setOrigin({ 633 / 2.f,534 / 2.f });
	m_sprite[SLASH_UP].setOrigin({ 238 / 2.f,511 / 2.f });
	m_sprite[SLASH_DOWN].setOrigin({ 238 / 2.f,511 / 2.f });

	m_sprite[POWER].setOrigin({ 256 / 2.f, 256 / 2.f });
}

ParticleData::~ParticleData()
{

}

void ParticleData::Update(float _dt)
{
	for (size_t i = 0; i < m_particleTab.size(); i++)
	{
		//Update the time the particule has been alive for
		m_particleTab[i]->AddLifeTime(_dt);

		m_particleTab[i]->Update(_dt);

		if (m_particleTab[i]->GetLifeTime() >= m_particleTab[i]->GetLifespan())
		{
			//We switch the last particule in the vector with the one who need to be destroyed
			m_particleTab[i] = m_particleTab.back();
			m_particleTab.pop_back();
		}
	}

	for (size_t i = 0; i < m_particleTabMiddleground.size(); i++)
	{
		//Update the time the particule has been alive for
		m_particleTabMiddleground[i]->AddLifeTime(_dt);

		m_particleTabMiddleground[i]->Update(_dt);

		if (m_particleTabMiddleground[i]->GetLifeTime() >= m_particleTabMiddleground[i]->GetLifespan())
		{
			//We switch the last particule in the vector with the one who need to be destroyed
			m_particleTabMiddleground[i] = m_particleTabMiddleground.back();
			m_particleTabMiddleground.pop_back();
		}
	}

	for (int i = 0; i < TOTAL_PARTICLE_TYPE; i++)
	{
		m_cooldown[i] -= _dt;
	}
}

void ParticleData::Draw(sf::RenderWindow& _window)
{
	for (size_t i = 0; i < m_particleTab.size(); i++)
	{
		m_particleTab[i]->Draw(_window, m_sprite[m_particleTab[i]->GetType()]);
	}
}

void ParticleData::DrawMiddleground(sf::RenderWindow& _window)
{
	for (size_t i = 0; i < m_particleTabMiddleground.size(); i++)
	{
		m_particleTabMiddleground[i]->Draw(_window, m_sprite[m_particleTabMiddleground[i]->GetType()]);
	}
}

//Do not use
void ParticleData::CreateParticle(ParticleType _type, unsigned int _amount)
{
	for (int i = 0; i < 10; i++)
	{
		m_particleTab.push_back(new BloodParticle({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2 }, { 1,0 }, 1, 50));
	}
}

void ParticleData::ClearAllParticle()
{
	for (auto i : m_particleTab)
	{
		m_particleTab.pop_back();
	}
	for (auto i : m_particleTabMiddleground)
	{
		m_particleTabMiddleground.pop_back();
	}
}

void ParticleData::AddParticle(Particle& _particle)
{
	m_particleTab.push_back(&_particle);
}

void ParticleData::AddParticleMiddleground(Particle& _particle)
{
	m_particleTabMiddleground.push_back(&_particle);
}

#pragma endregion

#pragma region Particle creator
void ParticleData::CreateBloodParticle(sf::Vector2f _pos, sf::Vector2f _dir, float _maxSpeed, float _lifespan, unsigned int _nbParticle)
{
	for (int i = 0; i < _nbParticle; i++)
	{
		BloodParticle* tempParticle = new BloodParticle(_pos, _dir, _maxSpeed, _lifespan);
		AddParticle(*tempParticle);
	}
}

void ParticleData::CreateWheatParticle(sf::Vector2f _pos, sf::Vector2f _dir, float _maxSpeed, float _lifespan, unsigned int _nbParticle)
{
	for (int i = 0; i < _nbParticle; i++)
	{
		WheatParticle* tempParticle = new WheatParticle(_pos, _dir, _maxSpeed, _lifespan);
		AddParticle(*tempParticle);
	}
}

void ParticleData::CreateHookConnectParticle(sf::Vector2f _pos, float _lifespan)
{
	float angle = 0;
	for (int i = 0; i < 8; i++)
	{
		angle = i * 360.f / 8.f;
		HookConnectParticle* tempParticle = new HookConnectParticle(_pos, _lifespan, angle);
		AddParticle(*tempParticle);
	}
	HookCircleParticle* tempParticle = new HookCircleParticle(_pos, _lifespan, m_sprite[HOOK_CIRCLE], m_anim[HOOK_CIRCLE]);
	AddParticle(*tempParticle);
}

void ParticleData::CreateLeafParticle(sf::Vector2f _pos, float _lifespan, unsigned int _nbParticle, bool _skipCooldown = false, bool _canSpawnInBackground = false, float _cooldown = 0)
{
	if (m_cooldown[LEAF1] <= 0 || _skipCooldown)
	{
		for (int i = 0; i < _nbParticle; i++)
		{
			LeafParticle* tempParticle = new LeafParticle(_pos, _lifespan);
			if (_canSpawnInBackground && rand() % 2)
			{
				AddParticleMiddleground(*tempParticle);
			}
			else
			{
				AddParticle(*tempParticle);
			}
		}

		if (!_skipCooldown)
		{
			m_cooldown[LEAF1] = _cooldown;
		}
	}
}

void ParticleData::CreateWalkDustParticle(sf::Vector2f _pos, int _dir, sf::Color _color, float _lifespan, unsigned int _nbParticle, bool _skipCooldown = false, float _cooldown = 0)
{
	if (m_cooldown[WALK_DUST] <= 0 || _skipCooldown)
	{
		for (int i = 0; i < _nbParticle; i++)
		{
			WalkDustParticle* tempParticle = new WalkDustParticle(_pos, _dir, _color, _lifespan);
			AddParticle(*tempParticle);
		}

		if (!_skipCooldown)
		{
			m_cooldown[WALK_DUST] = _cooldown;
		}
	}
}

void ParticleData::CreatePowerParticle(sf::Vector2f _pos, float _lifespan, sf::Color _color, unsigned int _nbParticle)
{
	for (int i = 0; i < _nbParticle; i++)
	{
		PowerParticle* tempParticle = new PowerParticle(_pos, _lifespan, _color, m_sprite[POWER], m_anim[POWER]);
		AddParticle(*tempParticle);
	}
}

void ParticleData::CreateBossScreamParticle(sf::Vector2f _pos, float _lifespan, float _cooldown)
{
	if (m_cooldown[BOSS_SCREAM] <= 0)
	{
		BossScreamParticle* tempParticle = new BossScreamParticle(_pos, _lifespan);
		AddParticle(*tempParticle);

		m_cooldown[BOSS_SCREAM] = _cooldown;
	}
}

void ParticleData::CreateDustDeathParticle(sf::Vector2f _pos, float _maxSpeed, float _lifespan, float _cooldown)
{
	if (m_cooldown[DUST_DEATH] <= 0)
	{
		DustDeathParticle* tempParticle = new DustDeathParticle(_pos, _lifespan, _maxSpeed);
		AddParticle(*tempParticle);

		m_cooldown[DUST_DEATH] = _cooldown;
	}
}

void ParticleData::CreateStarSuckedParticle(sf::Vector2f _pos, sf::Vector2f _posToReach, float _maxSpeed, float _lifespan, float _cooldown)
{
	if (m_cooldown[STAR_SUCKED] <= 0)
	{
		StarSuckedParticle* tempParticle = new StarSuckedParticle(_pos, _posToReach, _lifespan, _maxSpeed, m_shader[STAR_SUCKED]);
		AddParticleMiddleground(*tempParticle);

		m_cooldown[STAR_SUCKED] = _cooldown;
	}
}

void ParticleData::CreateAttackParticle(sf::Vector2f _pos, ParticleType _slashDir)
{
	AttackParticle* tempParticle = new AttackParticle(_pos, 1, _slashDir, &m_sprite[_slashDir], m_anim[_slashDir]);
	AddParticle(*tempParticle);
}
#pragma endregion

#pragma region Blood particle
BloodParticle::BloodParticle(sf::Vector2f _pos, sf::Vector2f _vectorDir, int _maxSpeed, float _lifespan) : Particle(_pos, _lifespan)
{
	sf::Vector2f unitVec = VecDirToVecUnit(_vectorDir);

	m_speed.x = unitVec.x * (rand() % _maxSpeed);
	m_speed.y = unitVec.y * (rand() % _maxSpeed);

	m_angle = 0;

	m_lifeTime += rand() / (float)RAND_MAX * _lifespan;

	m_type = (ParticleType)(rand() % (BLOOD4 - BLOOD1 + 1) + BLOOD1);
}

BloodParticle::~BloodParticle()
{

}

void BloodParticle::Update(float _dt)
{
	m_speed.x -= m_speed.x * _dt;
	m_speed.y += GRAVITY / 1.5 * _dt;
	m_pos += m_speed * _dt;
	m_angle = RadToDeg(acosf(VecDirToVecUnit(m_speed).x));

	m_angle = RadToDeg(atan2f(m_speed.y, m_speed.x));
}

void BloodParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setScale(1 + abs(VecDirToVecUnit(m_speed).x) * 2.f, 1);

	_sprite.setPosition(m_pos);
	_sprite.setRotation(m_angle);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Wheat particle
WheatParticle::WheatParticle(sf::Vector2f _pos, sf::Vector2f _vectorDir, int _maxSpeed, float _lifespan) : Particle(_pos, _lifespan)
{
	sf::Vector2f unitVec = VecDirToVecUnit(_vectorDir);

	m_speed.x = unitVec.x * (rand() % _maxSpeed);
	m_speed.y = unitVec.y * (rand() % _maxSpeed);

	m_angle = 0;

	m_lifeTime += rand() / (float)RAND_MAX * _lifespan;

	m_type = (ParticleType)(WHEAT);
}

WheatParticle::~WheatParticle()
{

}

void WheatParticle::Update(float _dt)
{
	m_speed.x -= m_speed.x * _dt;
	m_speed.y += GRAVITY / 1.5 * _dt;
	m_pos += m_speed * _dt;
	m_angle = RadToDeg(acosf(VecDirToVecUnit(m_speed).x));

	m_angle = RadToDeg(atan2f(m_speed.y, m_speed.x));
}

void WheatParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setScale(1 + abs(VecDirToVecUnit(m_speed).x) * 2.f, 1);

	_sprite.setPosition(m_pos);
	_sprite.setRotation(m_angle);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Hook particle
HookConnectParticle::HookConnectParticle(sf::Vector2f _pos, float _lifespan, float _angle) : Particle(_pos, _lifespan)
{
	m_angle = _angle;
	m_speed = { cosf(DegToRad(_angle)) * 250, sinf(DegToRad(_angle)) * 250 };

	m_type = HOOK_CONNECT;
}

HookConnectParticle::~HookConnectParticle()
{

}

void HookConnectParticle::Update(float _dt)
{
	m_pos += m_speed * _dt;

	m_angle = RadToDeg(atan2f(m_speed.y, m_speed.x));
}

HookCircleParticle::HookCircleParticle(sf::Vector2f _pos, float _lifespan, sf::Sprite& _ptrSprite, Anim& _anim) : Particle(_pos, _lifespan)
{
	m_ptrSprite = &_ptrSprite;
	m_anim = _anim;

	m_type = HOOK_CIRCLE;
}

HookCircleParticle::~HookCircleParticle()
{

}

void HookCircleParticle::Update(float _dt)
{
	Animation(*m_ptrSprite, &m_anim, 0, _dt);

	m_spriteFrame = m_ptrSprite->getTextureRect();
}

void HookCircleParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setTextureRect(m_spriteFrame);

	_sprite.setPosition(m_pos);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Leaf particle
LeafParticle::LeafParticle(sf::Vector2f _pos, float _lifespan) : Particle(_pos, _lifespan)
{
	m_speed.x = rand() % 70 + 10;
	m_speed.y = GRAVITY * 0.1f;

	m_lifeTime += rand() / (float)RAND_MAX * _lifespan;

	m_type = (ParticleType)(rand() % (LEAF3 - LEAF1 + 1) + LEAF1);
}

LeafParticle::~LeafParticle()
{

}

void LeafParticle::Update(float _dt)
{
	m_pos += m_speed * _dt;
}

void LeafParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	int transparancy = 255 * (1 - m_lifeTime / m_lifespan);
	_sprite.setColor(sf::Color(255, 255, 255, transparancy));
	_sprite.setPosition(m_pos);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Walk dust particle
WalkDustParticle::WalkDustParticle(sf::Vector2f _pos, int _direction, sf::Color _color, float _lifespan) : Particle(_pos, _lifespan)
{
	m_speed.x = _direction * (rand() % 70 + 10);
	m_speed.y = -GRAVITY * ((rand() % 100 + 1) / 1000.f);

	m_color = _color;

	m_type = WALK_DUST;
}

WalkDustParticle::~WalkDustParticle()
{

}

void WalkDustParticle::Update(float _dt)
{
	m_pos += m_speed * _dt;
}

void WalkDustParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	int transparancy = 255 * (1 - m_lifeTime / m_lifespan);
	m_color.a = transparancy;
	_sprite.setColor(m_color);
	_sprite.setPosition(m_pos);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Power particle
PowerParticle::PowerParticle(sf::Vector2f _pos, float _lifespan, sf::Color _color, sf::Sprite& _ptrSprite, Anim& _anim) : Particle(_pos, _lifespan)
{
	m_speed.x = (rand() % 50) - 25;
	m_speed.y = (rand() % 50) - 25;

	m_color = _color;

	m_type = POWER;

	m_ptrSprite = &_ptrSprite;
	m_anim = _anim;
}

PowerParticle::~PowerParticle()
{

}

void PowerParticle::Update(float _dt)
{
	m_pos += m_speed * _dt;
	Animation(*m_ptrSprite, &m_anim, 0, _dt);
	m_spriteFrame = m_ptrSprite->getTextureRect();
}

void PowerParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	int transparancy = 255 * (1 - m_lifeTime / m_lifespan);
	m_color.a = transparancy;
	_sprite.setColor(m_color);
	_sprite.setPosition(m_pos);
	_sprite.setTextureRect(m_spriteFrame);
	_window.draw(_sprite);
}
#pragma endregion

#pragma region Boss Scream particle
BossScreamParticle::BossScreamParticle(sf::Vector2f _pos, float _lifespan) : Particle(_pos, _lifespan)
{
	m_type = BOSS_SCREAM;
}

BossScreamParticle::~BossScreamParticle()
{

}

void BossScreamParticle::Update(float _dt)
{
	m_scale = exp2f(4.f * (m_lifeTime / m_lifespan)) - 0.5f;

	m_transparancy = cos((m_lifeTime / m_lifespan) * M_PI_2) * 255;
}

void BossScreamParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setScale({ m_scale, m_scale });
	_sprite.setColor(sf::Color(255, 255, 255, m_transparancy));
	_sprite.setPosition(m_pos);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Dust Death particle
DustDeathParticle::DustDeathParticle(sf::Vector2f _pos, float _lifespan, int _maxSpeed) : Particle(_pos, _lifespan)
{
	m_scale = rand() / (float)RAND_MAX;
	if (m_scale <= 0.5)
	{
		m_scale = 0.51;
	}
	m_maxTransparancy = rand() % 255;

	m_speed.x = -_maxSpeed + (rand() % _maxSpeed * 2);
	m_speed.y = -_maxSpeed + (rand() % _maxSpeed * 2);

	m_lifeTime = rand() / (float)RAND_MAX * m_lifespan;
	m_lifespan -= m_lifeTime;
	m_lifeTime = 0;

	m_type = DUST_DEATH;
}

DustDeathParticle::~DustDeathParticle()
{

}

void DustDeathParticle::Update(float _dt)
{
	m_transparancy = sin((m_lifeTime / m_lifespan) * M_PI) * 255;

	m_pos += m_speed * _dt;

}

void DustDeathParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setScale({ m_scale, m_scale });
	_sprite.setColor(sf::Color(255, 255, 255, m_transparancy));
	_sprite.setPosition(m_pos);

	_window.draw(_sprite);
}
#pragma endregion

#pragma region Star Sucked particle
StarSuckedParticle::StarSuckedParticle(sf::Vector2f _pos, sf::Vector2f _posToReach, float _lifespan, int _maxSpeed, sf::Shader& _shader) : Particle(_pos, _lifespan)
{
	m_posToReach = _posToReach;
	m_posOrigin = _pos;
	m_vectorDir = VecDirToVecUnit(_posToReach - _pos);
	m_transparancy = 0.f;

	m_speed = { (float)_maxSpeed, (float)_maxSpeed };

	m_ptrShader = &_shader;

	m_type = STAR_SUCKED;
}

StarSuckedParticle::~StarSuckedParticle()
{

}

void StarSuckedParticle::Update(float _dt)
{
	m_pos += sf::Vector2f{ m_vectorDir.x * m_speed.x, m_vectorDir.y * m_speed.y } *_dt;
	m_transparancy = 255 * (m_lifeTime / m_lifespan) * 2;
	if (m_transparancy > 255)
	{
		m_transparancy = 255;
	}
	//m_ptrShader->loadFromFile("Shaders/Map/Death/Streching.frag", sf::Shader::Fragment);
}

void StarSuckedParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setColor(sf::Color(255, 255, 255, m_transparancy));
	_sprite.setPosition(m_pos);

	_window.draw(_sprite);

	//Shader moment
	//m_ptrShader->setUniform("posToReach", m_posToReach);
	//m_ptrShader->setUniform("posOrigin", m_posOrigin);
	//m_ptrShader->setUniform("posCurrent", m_pos);
	//m_ptrShader->setUniform("alpha", m_transparancy);
	//m_ptrShader->setUniform("vecDir", m_vectorDir);

	//_window.draw(_sprite, &*m_ptrShader);
}
#pragma endregion

#pragma region Attack particle
AttackParticle::AttackParticle(sf::Vector2f _pos, float _lifespan, ParticleType _slashDir, sf::Sprite* _ptrSprite, Anim& _anim) : Particle(_pos, _lifespan)
{
	m_ptrSprite = _ptrSprite;
	m_anim = _anim;


	m_type = _slashDir;

	m_lifespan = (float)m_anim.frameNb / m_anim.framerate;
	//std::cout << "frameNb : " << m_ptrAnim[m_type].frameNb << "\tframerate: " << m_ptrAnim[m_type].framerate << std::endl;
	//std::cout << "lifespan: " << m_lifespan << std::endl;
}

AttackParticle::~AttackParticle()
{

}

void AttackParticle::Update(float _dt)
{
	Animation(*m_ptrSprite, &m_anim, 0, _dt);

	//std::cout << m_ptrAnim->timeAnim << std::endl;

	m_spriteFrame = m_ptrSprite->getTextureRect();
}

void AttackParticle::Draw(sf::RenderWindow& _window, sf::Sprite& _sprite)
{
	_sprite.setTextureRect(m_spriteFrame);

	_sprite.setPosition(m_pos);

	_window.draw(_sprite);
}
#pragma endregion