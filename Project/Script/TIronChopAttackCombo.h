#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronChopAttackCombo :
	public CState
{
private:
    CIronmaceScript* m_Script;
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

