#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleMissile_Close :
	public CState
{
private:
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleMissile_Close();
    ~TCastleMissile_Close();
};

