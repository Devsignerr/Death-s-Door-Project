#pragma once
#include <Engine/CState.h>
class TCrowCutScene :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowCutScene();
    ~TCrowCutScene();
};

