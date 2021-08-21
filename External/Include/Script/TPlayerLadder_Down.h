#pragma once
#include <Engine/CState.h>

class TPlayerLadder_Down :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerLadder_Down();
    ~TPlayerLadder_Down();
};

