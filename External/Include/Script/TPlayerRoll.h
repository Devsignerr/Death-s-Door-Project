#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerRoll :
	public CState
{
private:
    bool m_IsRollSlash;
    CPlayerScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerRoll();
    ~TPlayerRoll();
};

