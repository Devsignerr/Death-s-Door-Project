#pragma once
#include <Engine/CState.h>

class TCrowSliding;
class TCrowStomp :
    public CState
{
private:
    TCrowSliding* m_SlidingState;
    void CheckBehind();
    int m_DirCheck;
    float m_Speed;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowStomp();
    ~TCrowStomp();
};

