#pragma once
#include <Engine/CState.h>

class TPlayerLadder_Up :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerLadder_Up();
    ~TPlayerLadder_Up();
};

