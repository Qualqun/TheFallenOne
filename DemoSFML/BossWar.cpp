#include "BossWar.h"

BossWarData::BossWarData(b2World* _world, sf::Vector2f _vec)
{
	timer = 0;
	B2Dinfo tempBody;
	tempBody.userData.fixtureName = "Boss";
	tempBody.type = b2_dynamicBody;
	tempBody.shape = POLYGON;
	tempBody.pos = { _vec.x / 64, _vec.y / 64 };
	tempBody.points =
	{
	b2Vec2(-BOSS_WIDTH_WAR / 128 ,  BOSS_HEIGHT_WAR / 128),
	b2Vec2(-BOSS_WIDTH_WAR / 128,-BOSS_HEIGHT_WAR / 128),
	b2Vec2(BOSS_WIDTH_WAR / 128, -BOSS_HEIGHT_WAR / 128),
	b2Vec2(BOSS_WIDTH_WAR / 128 , BOSS_HEIGHT_WAR / 128),
	};
	tempBody.radius = 2.0f;
	tempBody.density = 1;
	tempBody.friction = 1;
	b2Body* finalBody = CreateBodyB2D(_world, tempBody);

	b2Filter filter;
	filter.categoryBits = 1;
	filter.groupIndex = -1;
	SetBody(finalBody);
	SetGlobalTimer(0);
	timerTp = 3.f;

	SetBody(finalBody);
}

void BossWarData::Update(float _dt, b2World* _world)
{

}

void BossWarData::Display(sf::RenderWindow& _window)
{

}