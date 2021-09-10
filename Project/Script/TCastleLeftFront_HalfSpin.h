#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleLeftFront_HalfSpin :
	public CState
{
private:

    CCastleScript* m_Script;
    bool           m_bImpacted;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleLeftFront_HalfSpin();
    ~TCastleLeftFront_HalfSpin();
};

