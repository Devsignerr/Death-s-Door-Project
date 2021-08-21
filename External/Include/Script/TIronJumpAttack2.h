#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronJumpAttack2 :
	public CState
{
private:
    CIronmaceScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronJumpAttack2();
    ~TIronJumpAttack2();
};

