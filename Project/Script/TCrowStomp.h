#pragma once
#include <Engine/CState.h>

class TCrowSliding;
class TCrowJump;
class TCrowStomp :
    public CState
{
private:
    TCrowSliding*   m_SlidingState;
    TCrowJump*      m_JumpState;
    int             m_DirCheck;
    float           m_Speed;

    Vec3            m_Pos;

private:
    void CheckBehind();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowStomp();
    ~TCrowStomp();
};

