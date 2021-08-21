#pragma once
#include <Engine/CState.h>

class TPlayerHook :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerHook();
    ~TPlayerHook();
};

