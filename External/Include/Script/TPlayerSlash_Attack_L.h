#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerSlash_Attack_L :
	public CState
{
private:
    CPlayerScript* m_Script;
    bool m_IsRightSlash;
    int  m_LimitAttackCount;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerSlash_Attack_L();
    ~TPlayerSlash_Attack_L();
};

