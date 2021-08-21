#pragma once
#include <Engine/CState.h>

class TPlayerCharge_L :
	public CState
{
private:
    bool m_IsChargeStay;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerCharge_L();
    ~TPlayerCharge_L();
};

