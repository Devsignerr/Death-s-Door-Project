#pragma once
#include <Engine/CState.h>

class TIronJumpAttack :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronJumpAttack();
    ~TIronJumpAttack();
};

