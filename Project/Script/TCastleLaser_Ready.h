#pragma once
#include <Engine/CState.h>

class CGameObject;
class CCastleScript;
class TCastleLaser_Ready :
	public CState
{
private:
    float m_LaserTime;
    float m_LaserAimTime;
    CGameObject* m_LaserPoint;
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleLaser_Ready();
    ~TCastleLaser_Ready();
};

