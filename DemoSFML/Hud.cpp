#include "Hud.hpp"

HudData::HudData(PlayerData& _player)
{
	m_back = LoadSprite("Sprites/Hud/Back.png", true);
	m_chain = LoadSprite("Sprites/Hud/Chain.png", true);
	m_tier[0] = LoadSprite("Sprites/Hud/Tier.png", true);
	m_tier[1] = LoadSprite("Sprites/Hud/Tier.png", true);
	m_hp = LoadSprite("Sprites/Hud/Health.png", true);
	m_hpOutline = LoadSprite("Sprites/Hud/HealthOutline.png", true);
	m_timerHit = 1.f;
	m_back.setPosition({ SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5 });
	m_chain.setPosition(m_back.getPosition());
	m_tier[0].setPosition(m_back.getPosition());
	m_tier[1].setPosition(m_back.getPosition());
	m_hp.setPosition(m_back.getPosition());
	m_hpOutline.setPosition(m_back.getPosition());
	m_YRectPreview = 0;
	m_XRectPreview = 410;
	m_tier[0].setRotation(-90);

	SetSpritePower(_player.GetSpritePower());

	m_shader.loadFromFile("Shaders/Hud/Shader.frag", sf::Shader::Fragment);

	SetHealth(m_healthMax);

	//BOSS LIFEBAR INFO
	/*m_bossHpBar = LoadSprite("Sprites/Hud/LifeBar/hpBossBar.png", true);
	m_bossHpBar.setPosition(1700, 216);
	m_bossHpOverlayRed = LoadSprite("Sprites/Hud/LifeBar/hpBoss.png", true);
	m_bossHpOverlayRed.setPosition(1700, 216);
	m_bossHpPreview = LoadSprite("Sprites/Hud/LifeBar/hpBossPreview.png", true);
	m_bossHpPreview.setPosition(1700, 216);*/
	//PLAYER LIFEBAR INFO
	m_playerHpBar = LoadSprite("Sprites/Hud/LifeBar/hpPlayerBar.png", false);
	m_playerHpBar.setPosition(100, 100);
	m_playerHpOverlayRed = LoadSprite("Sprites/Hud/LifeBar/hpPlayer.png", false);
	m_playerHpOverlayRed.setPosition(100, 100);
	m_playerHpPreview = LoadSprite("Sprites/Hud/LifeBar/hpPlayerPreview.png", false);
}

HudData::~HudData()
{

}

void HudData::Update(float _dt, CamData& _camData, BossData& _bossData, PlayerData& _playerData)
{
	sf::Vector2f camOffset = _camData.GetPosition() - _camData.GetCamera().getSize() / 2.f;
	sf::Vector2f newHudPos = sf::Vector2f{ SCREEN_WIDTH / 10, SCREEN_HEIGHT / 7 } + camOffset;
	if (&_bossData != nullptr)
	{
		UpdateBossLifeBar(_dt, _bossData, camOffset);

	}
	UpdatePlayerLifeBar(_dt, &_playerData, camOffset);

	float bossPourcentageHp = float(float(_bossData.GetHp()) / float(_bossData.GetMaxHp()));
	m_back.setPosition(newHudPos);
	m_chain.setPosition(m_back.getPosition());
	m_tier[0].setPosition(m_back.getPosition());
	m_tier[1].setPosition(m_back.getPosition());
	m_hp.setPosition(m_back.getPosition());
	m_hpOutline.setPosition(m_back.getPosition());

	m_tier[1].setRotation(RadToDeg(-m_angleHealth + M_PI_2));
}
void HudData::UpdatePlayerLifeBar(float _dt, PlayerData* _playerData, sf::Vector2f _offSet)
{
	float playerPourcentageHp = float(float(_playerData->GetHp() / float(MAX_HP)));
	// HP BAR POSITION

	m_playerHpBar.setPosition({ _offSet.x + 9, _offSet.y + 10 });
	m_playerHpPreview.setPosition({ _offSet.x + 261, _offSet.y + 106 });
	m_playerHpOverlayRed.setPosition({ _offSet.x + 261, _offSet.y + 106 });

	m_playerHpOverlayRed.setTextureRect({ 0 , 0 ,int(410 * playerPourcentageHp) , 39 });

	if (m_playerHpPreview.getTextureRect().width >= m_playerHpOverlayRed.getTextureRect().width && _playerData->m_timerDamage <= 0.f)
	{
		m_XRectPreview -= _dt * 150;
		m_playerHpPreview.setTextureRect({ m_playerHpPreview.getTextureRect().left , m_playerHpPreview.getTextureRect().top , int(m_XRectPreview) , m_playerHpPreview.getTextureRect().height });
	}
}
void HudData::UpdateBossLifeBar(float _dt, BossData& _bossData, sf::Vector2f _offSet)
{
//	float bossPourcentageHp = float(float(_bossData.GetHp()) / float(_bossData.GetMaxHp()));
//	//std::cout << bossPourcentageHp << std::endl;
//	// HP BAR POSITION
//	m_bossHpBar.setPosition({ _offSet.x + 1700, _offSet.y + 500 });
//
//	m_bossHpOverlayRed.setPosition({ _offSet.x + 1700, _offSet.y + 532 + (690 * abs(1 - bossPourcentageHp)) });
//	m_bossHpOverlayRed.setTextureRect({ 0 , int(0 + (690 * (1 - bossPourcentageHp))) ,29 , 690 });
//
//	m_bossHpPreview.setPosition({ _offSet.x + 1700, _offSet.y + 532 + m_YRectPreview });
//
//	if (m_bossHpPreview.getTextureRect().top <= m_bossHpOverlayRed.getTextureRect().top && _bossData.GetTimerDamage() <= 0.f)
//	{
//		m_YRectPreview += _dt * 200;
//		m_bossHpPreview.setTextureRect({ m_bossHpPreview.getTextureRect().left , int(m_YRectPreview) , m_bossHpPreview.getTextureRect().width , 690 });
//	}

}
void HudData::Draw(sf::RenderWindow& _window, PlayerData& _player)
{
	//_window.draw(m_back);
	//_window.draw(m_hp, &m_shader);
	//_window.draw(m_chain);
	//_window.draw(m_hpOutline);
	//_window.draw(m_tier[0]);
	//_window.draw(m_tier[1]);

	if (_player.m_powerData[_player.GetSelectedPower()].m_cooldown > 0)
	{
		m_spritePowerHUD[_player.GetSelectedPower()].setColor(sf::Color(100, 100, 100));
	}
	else
	{
		m_spritePowerHUD[_player.GetSelectedPower()].setColor(sf::Color(255, 255, 255));
	}
	/*_window.draw(m_bossHpPreview);
	_window.draw(m_bossHpOverlayRed);
	_window.draw(m_bossHpBar);*/

	_window.draw(m_playerHpPreview);
	_window.draw(m_playerHpOverlayRed);
	_window.draw(m_playerHpBar);
	BlitSprite(m_spritePowerHUD[_player.GetSelectedPower()], { 180 + m_playerHpBar.getPosition().x , 180 + m_playerHpBar.getPosition().y }, 0, _window);
}

void HudData::SetHealth(float _health)
{
	m_health = _health;

	//Magical value needed for the shader
	m_angleHealth = m_health / m_healthMax * M_PI * 2 - M_PI;
	m_shader.setUniform("value", m_angleHealth);
}