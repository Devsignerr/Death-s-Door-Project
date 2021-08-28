#pragma once
#include <Engine/CState.h>

class CCastleScript;
class CGameObject;
class TCastleFly_Stay :
	public CState
{
private:
    CGameObject* m_LaserPoint;
    bool         m_IsRotLaserPoint;
    CCastleScript* m_Script;
private:
    bool TurnToLaserPoint(Vec3 _TargetPos, float _RotSpeed);
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleFly_Stay();
    ~TCastleFly_Stay();
};

