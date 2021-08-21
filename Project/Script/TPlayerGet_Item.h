#pragma once
#include <Engine/CState.h>

class TPlayerGet_Item :
	public CState
{
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerGet_Item();
    ~TPlayerGet_Item();
};

