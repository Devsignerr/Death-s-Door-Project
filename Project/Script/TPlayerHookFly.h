#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerHookFly :
	public CState
{
private:
    CPlayerScript* m_Script;
    float           m_fFlySpeed;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerHookFly();
    ~TPlayerHookFly();
};

