#pragma once
#include <Engine/CState.h>

class TCastleMissile_Open :
    public CState
{
private:
    float m_MissileTime;
    float m_MissileDelay;
private:
    void update() override;

    void Enter() override;
    void Exit() override;
    void MissileCreate(Vec3 _Pos, Vec3 _Dir);
public:
    TCastleMissile_Open();
    ~TCastleMissile_Open();
};

