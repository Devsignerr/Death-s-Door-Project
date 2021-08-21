#pragma once
#include <Engine/CState.h>
class TCrowWalk :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowWalk();
    ~TCrowWalk();

};

