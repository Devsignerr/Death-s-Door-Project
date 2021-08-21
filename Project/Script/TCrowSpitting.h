#pragma once
#include <Engine/CState.h>
class TCrowSpitting :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowSpitting();
    ~TCrowSpitting();
};

