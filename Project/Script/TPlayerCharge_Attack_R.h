#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerCharge_Attack_R :
	public CState
{
private:
    bool m_IsChargeAttack;
    CPlayerScript* m_Script;
    int m_ColOnOffCheck;

public:
    bool GetIsChargeAttack() { return m_IsChargeAttack; }
    void ResetIsChargeAttack() { m_IsChargeAttack = false; }

private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerCharge_Attack_R();
    ~TPlayerCharge_Attack_R();
};

