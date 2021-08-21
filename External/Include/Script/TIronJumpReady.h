#pragma once
#include <Engine/CState.h>

class TIronJumpReady :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronJumpReady();
    ~TIronJumpReady();
};

