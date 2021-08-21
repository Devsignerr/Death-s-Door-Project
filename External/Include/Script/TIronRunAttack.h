#pragma once
#include <Engine/CState.h>

class TIronRunAttack :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronRunAttack();
    ~TIronRunAttack();
};

