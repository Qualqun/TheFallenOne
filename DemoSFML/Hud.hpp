#ifndef HUD_HPP
#define HUD_HPP

#include "Animation.hpp"
#include "Player.h"
#include "Camera.hpp"


#define WIDTH_HP_BAR 740

class PlayerData;
class BossData;

class HudData
{
private:
	sf::Sprite m_back;
	sf::Sprite m_chain;
	sf::Sprite m_tier[2];
	sf::Sprite m_hp;
	sf::Sprite m_hpOutline;

	sf::Shader m_shader;

	float m_healthMax = 100.f;
	float m_health = m_healthMax;
	float m_timerHit;

	float m_angleHealth = 0;
	float m_healthBoss;
	float m_healthBossPreview;
	float m_YRectPreview;
	float m_XRectPreview;


	sf::Sprite m_bossHpBar;
	sf::Sprite m_bossHpOverlayRed;
	sf::Sprite m_bossHpPreview;

	sf::Sprite m_playerHpBar;
	sf::Sprite m_playerHpOverlayRed;
	sf::Sprite m_playerHpPreview;


	std::vector<sf::Sprite> m_spritePowerHUD;
public:
	HudData(PlayerData& _player);
	~HudData();

	void Update(float _dt, CamData& _camData, BossData& _bossData, PlayerData& _playerData);
	void UpdatePlayerLifeBar(float _dt, PlayerData* _playerData, sf::Vector2f _offSet);
	void UpdateBossLifeBar(float _dt, BossData& _bossData, sf::Vector2f _offSet);
	void Draw(sf::RenderWindow& _window, PlayerData& _player);

	void SetHealth(float _health);

	void SetSpritePower(std::vector<sf::Sprite> _spriteVec) { m_spritePowerHUD = _spriteVec; }
};

#endif