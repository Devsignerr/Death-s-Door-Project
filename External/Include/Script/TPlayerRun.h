#pragma once
#include <Engine/CState.h>
class TPlayerRun :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerRun();
    ~TPlayerRun();
};

