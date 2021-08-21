#pragma once
#include <Engine/CState.h>
class TCrowSlidingReady :
    public CState
{
private:
    Vec3 m_LastDir;
public:
    Vec3 GetLastDir() { return m_LastDir; }
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowSlidingReady();
    ~TCrowSlidingReady();
};

