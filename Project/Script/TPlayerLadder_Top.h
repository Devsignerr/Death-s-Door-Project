#pragma once
#include <Engine/CState.h>

class TPlayerLadder_Top :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerLadder_Top();
    ~TPlayerLadder_Top();
};

