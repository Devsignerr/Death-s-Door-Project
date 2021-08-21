#pragma once
#include <Engine/CState.h>

class TPlayerChargeMax_R :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerChargeMax_R();
    ~TPlayerChargeMax_R();
};

