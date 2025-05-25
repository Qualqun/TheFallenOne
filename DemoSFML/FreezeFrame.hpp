#ifndef FREEZEFRAME_HPP
#define FREEZEFRAME_HPP

#include "Collide.h"

class GameData;

class FreezeData
{
private:
    bool m_freezeFrame = false;
    float m_freezeTimer = 0;
    float m_freezeCooldown = 0;
public:


    FreezeData();
    ~FreezeData();

    void FreezeFrame(GameData& _gameData, float _time, float _cooldown);
    bool IsFrameFreezed() { return m_freezeFrame; }
    bool CanBeFreezed() { return (m_freezeCooldown <= 0) ? true : false; }

    void Update(float _dt);

    void UpdateFreezedFrame(float _dt);
};

#endif