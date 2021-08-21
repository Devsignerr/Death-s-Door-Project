#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerCharge_Attack_L :
	public CState
{

private:
    CPlayerScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerCharge_Attack_L();
    ~TPlayerCharge_Attack_L();
};

