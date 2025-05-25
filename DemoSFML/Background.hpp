#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Common.h"
#include "Animation.hpp"
enum MistAnimations
{
	AM,
	BA,
	TA,
	KA,
	M,
	TOTAL_MISTANIM
};
enum BossCount
{
	PANDEMICBOSS,
	FAMINEBOSS,
	WARBOSS,
	DEATHBOSS,
	JUMP
};
class BackgroundData
{
private:
	//Voir pour faire du parallaxe

	std::vector<sf::Sprite> m_background;
	std::vector<sf::Sprite> m_middleground;
	std::vector<sf::Sprite> m_foreground;

	//Shader vector
	std::vector<sf::Shader*> m_shaderBackground;
	std::vector<sf::Shader*> m_shaderMiddleground;
	std::vector<sf::Shader*> m_shaderForeground;

	MistAnimations m_mistTypeAnims;
	Anim m_mistAnim[TOTAL_MISTANIM];
	Anim m_mistAnim2[TOTAL_MISTANIM];

	int m_phaseBackground;

	sf::Sprite m_mistSprite;
	int m_currentMistAnim;
	int m_currentMistAnim2;

	float m_timer = 0.f;

public:
	BackgroundData();
	~BackgroundData();

	void Update(float _dt, sf::Vector2f _posCam);

	//Update Pandemic
	void AnimMist(float _dt);

	void AnimBackground(float _dt);

	void DrawBackground(sf::RenderWindow& _window, sf::Vector2f _posCam);
	void DrawMiddleground(sf::RenderWindow& _window, sf::Vector2f _posCam);
	void DrawForeground(sf::RenderWindow& _window, sf::Vector2f _posCam);

	//Draw Pandemic
	void DrawMist(sf::RenderWindow& _window, sf::Vector2f _posCam, int _nbLayer);


	void AddBackground(sf::Sprite& _sprite, sf::Vector2f _pos);
	void AddMiddleground(sf::Sprite& _sprite, sf::Vector2f _pos);
	void AddForeground(sf::Sprite& _sprite, sf::Vector2f _pos);
	//Shader overload
	void AddBackground(sf::Sprite& _sprite, sf::Vector2f _pos, sf::Shader& _shader);
	void AddMiddleground(sf::Sprite& _sprite, sf::Vector2f _pos, sf::Shader& _shader);
	void AddForeground(sf::Sprite& _sprite, sf::Vector2f _pos, sf::Shader& _shader);

	void LoadPandemicBackground();
	void LoadDeathBackground();
	void LoadJumpBackground();

	void CleanAllGround();
};

#endif