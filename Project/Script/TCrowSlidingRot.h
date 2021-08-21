#pragma once
#include <Engine/CState.h>

class TCrowSlidingRot :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowSlidingRot();
    ~TCrowSlidingRot();
};

