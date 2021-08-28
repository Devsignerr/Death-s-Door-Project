#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleFly_Finish :
	public CState
{
private:
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleFly_Finish();
    ~TCastleFly_Finish();
};

