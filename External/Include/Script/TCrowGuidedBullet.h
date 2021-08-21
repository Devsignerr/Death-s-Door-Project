#pragma once
#include <Engine/CState.h>
class TCrowGuidedBullet :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowGuidedBullet();
    ~TCrowGuidedBullet();
};

