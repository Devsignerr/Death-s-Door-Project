#pragma once
#include <Engine/CState.h>

class CCrowScript;
class TCrowRightSpin :
    public CState
{
private:
    Vec3 m_LastDir;
    CCrowScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowRightSpin();
    ~TCrowRightSpin();
};

