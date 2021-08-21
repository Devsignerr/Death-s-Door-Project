#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerArrow :
	public CState
{
private:
    CPlayerScript* m_Script;
    bool            m_BulletLimit;
private:
    void Attack();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerArrow();
    ~TPlayerArrow();
};

