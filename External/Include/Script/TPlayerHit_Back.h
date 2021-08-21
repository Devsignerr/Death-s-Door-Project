#pragma once
#include <Engine/CState.h>

class TPlayerHit_Back :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerHit_Back();
    ~TPlayerHit_Back();
};

