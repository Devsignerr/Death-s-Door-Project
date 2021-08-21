#pragma once
#include <Engine/CState.h>

class CCrowScript;
class TCrowRun :
	public CState
{
private:
    CCrowScript* m_Script;
    int m_DirCheck;
private:
    bool CheckBehind();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowRun();
    ~TCrowRun();
};

