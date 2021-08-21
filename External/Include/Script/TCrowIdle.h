#pragma once
#include <Engine/CState.h>
class TCrowIdle :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowIdle();
    ~TCrowIdle();
};

