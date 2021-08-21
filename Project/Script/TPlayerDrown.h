#pragma once
#include <Engine/CState.h>

class TPlayerDrown :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerDrown();
    ~TPlayerDrown();
};

