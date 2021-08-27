#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerHook :
	public CState
{
private:
    CPlayerScript* m_Script;
    bool            m_BulletLimit;

private:
    void Fly();
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerHook();
    ~TPlayerHook();
};

