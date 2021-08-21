#pragma once
#include <Engine/CState.h>

class TIronDeath :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronDeath();
    ~TIronDeath();
};

