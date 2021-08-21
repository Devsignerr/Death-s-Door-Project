#pragma once
#include <Engine/CState.h>

class TPlayerHit_Recover :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerHit_Recover();
    ~TPlayerHit_Recover();
};

