#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleRightFront_HalfSpin :
	public CState
{
private:
    CCastleScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleRightFront_HalfSpin();
    ~TCastleRightFront_HalfSpin();
};

