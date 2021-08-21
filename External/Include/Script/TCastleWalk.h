#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleWalk :
	public CState
{
private:
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleWalk();
    ~TCastleWalk();
};

