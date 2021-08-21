#pragma once
#include <Engine/CState.h>

class TCastleFly_Finish :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleFly_Finish();
    ~TCastleFly_Finish();
};

