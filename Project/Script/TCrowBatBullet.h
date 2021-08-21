#pragma once
#include <Engine/CState.h>

class TCrowBatBullet :
    public CState
{
private:
    float m_Reload;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowBatBullet();
    ~TCrowBatBullet();
};

