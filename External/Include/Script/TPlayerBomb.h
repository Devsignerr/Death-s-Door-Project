#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerBomb :
	public CState
{
private:
    CPlayerScript* m_Script;
    float          m_fRotateTime;
    float          m_fCurTime;
    float          m_fRadius;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerBomb();
    ~TPlayerBomb();
};

