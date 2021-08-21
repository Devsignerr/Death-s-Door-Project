#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleRightBack_HalfSpin :
	public CState
{
private:
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleRightBack_HalfSpin();
    ~TCastleRightBack_HalfSpin();
};

