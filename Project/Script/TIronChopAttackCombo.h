#pragma once
#include <Engine/CState.h>

class TIronChopAttackCombo :
	public CState
{
private:
    bool SpinAttackCheck();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronChopAttackCombo();
    ~TIronChopAttackCombo();
};

