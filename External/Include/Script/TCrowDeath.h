#pragma once
#include <Engine/CState.h>
class TCrowDeath :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowDeath();
    ~TCrowDeath();
};

