#pragma once
#include <Engine/CState.h>

class TPlayerCharge_R :
	public CState
{
private:
    bool m_IsChargeStay;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerCharge_R();
    ~TPlayerCharge_R();
};

