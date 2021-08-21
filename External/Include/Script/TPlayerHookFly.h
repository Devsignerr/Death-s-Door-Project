#pragma once
#include <Engine/CState.h>

class TPlayerHookFly :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerHookFly();
    ~TPlayerHookFly();
};

