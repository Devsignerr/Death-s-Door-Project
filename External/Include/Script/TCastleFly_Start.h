#pragma once
#include <Engine/CState.h>

class TCastleFly_Start :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleFly_Start();
    ~TCastleFly_Start();
};

