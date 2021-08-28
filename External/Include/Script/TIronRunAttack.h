#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronRunAttack :
	public CState
{
private:
    CIronmaceScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronRunAttack();
    ~TIronRunAttack();
};

