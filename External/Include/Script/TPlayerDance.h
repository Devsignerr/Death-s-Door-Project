#pragma once
#include <Engine/CState.h>

class TPlayerDance :
    public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerDance();
    ~TPlayerDance();
};

