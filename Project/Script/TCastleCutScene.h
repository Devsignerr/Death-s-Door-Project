#pragma once
#include <Engine/CState.h>

class TCastleCutScene :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleCutScene();
    ~TCastleCutScene();
};

