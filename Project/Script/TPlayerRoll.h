#pragma once
#include <Engine/CState.h>
class TPlayerRoll :
	public CState
{
private:
    bool m_IsRollSlash;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerRoll();
    ~TPlayerRoll();
};

