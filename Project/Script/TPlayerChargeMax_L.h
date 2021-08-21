#pragma once
#include <Engine/CState.h>

class TPlayerChargeMax_L :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerChargeMax_L();
    ~TPlayerChargeMax_L();
};

