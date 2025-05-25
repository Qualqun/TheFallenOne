#ifndef DEATHSTATE_H
#define DEATHSTATE_H

#include "Collide.h"

class GameData;

class DeathData
{
private:
	sf::Sprite m_background;
	sf::Sprite m_looseText;
	sf::Sprite m_backText[2];
	sf::Sprite m_choice[2];
	sf::Sprite m_select;

	

	sf::Shader m_selectShader;
	sf::Clock m_clockShader;
public:
	DeathData();
	~DeathData();

	void Update(float _dt, sf::RenderWindow& _window);
	void Draw(sf::RenderWindow& _window);

	void Event(sf::Event _event, sf::RenderWindow& _window, GameData& _gameData);

	sf::Sprite m_oversprite = LoadSprite("Sprites/DeathScreen/MenuDeath.png", false);
};

#endif