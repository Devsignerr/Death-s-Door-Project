#pragma once
#include <Engine/CState.h>

class TIronJumpFinish :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronJumpFinish();
    ~TIronJumpFinish();
};

