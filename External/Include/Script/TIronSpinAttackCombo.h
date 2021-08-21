#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronSpinAttackCombo :
	public CState
{
private:
    CIronmaceScript* m_Script;
private:
    bool ChopAttackCheck();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronSpinAttackCombo();
    ~TIronSpinAttackCombo();
};

