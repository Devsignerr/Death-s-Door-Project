#pragma once
#include <Engine/CState.h>
class TPlayerRun :
	public CState
{
private:
    float m_SoundTimer;
    int   m_iRunSoundChange;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerRun();
    ~TPlayerRun();
};

