#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerIdle :
	public CState
{
private:
    CPlayerScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerIdle();
    ~TPlayerIdle();
};

