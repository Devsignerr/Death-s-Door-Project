#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleLaser_Finish :
	public CState
{
private:
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleLaser_Finish();
    ~TCastleLaser_Finish();
};

