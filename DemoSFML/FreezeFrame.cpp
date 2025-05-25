#include "FreezeFrame.hpp"

FreezeData::FreezeData()
{

}

FreezeData::~FreezeData()
{

}

void FreezeData::Update(float _dt)
{
	if (m_freezeCooldown > 0)
	{
		m_freezeCooldown -= _dt;
	}
}

void FreezeData::UpdateFreezedFrame(float _dt)
{
	m_freezeTimer -= _dt;
	if (m_freezeTimer <= 0)
	{
		m_freezeFrame = false;
	}
}

void FreezeData::FreezeFrame(GameData& _gameData, float _time, float _cooldown)
{
	if (m_freezeCooldown <= 0)
	{
		m_freezeFrame = true;
		m_freezeTimer = _time;
		m_freezeCooldown = _cooldown;
	}
}