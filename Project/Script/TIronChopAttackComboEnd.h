#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronChopAttackComboEnd :
	public CState
{
    CIronmaceScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronChopAttackComboEnd();
    ~TIronChopAttackComboEnd();
};

