#pragma once
#include <Engine/CState.h>

class TCrowStandingDeath :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowStandingDeath();
    ~TCrowStandingDeath();
};

