#pragma once
#include <Engine/CState.h>

class CCrowScript;
class TCrowIdle :
    public CState
{
private:
    CCrowScript* m_Script;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowIdle();
    ~TCrowIdle();
};

