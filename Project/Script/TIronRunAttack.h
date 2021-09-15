#pragma once
#include <Engine/CState.h>

class CIronmaceScript;
class TIronRunAttack :
	public CState
{
private:
    CIronmaceScript* m_Script;
    bool			 m_bWalkSoundChange;
    float		     m_fSoundTimer;

private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronRunAttack();
    ~TIronRunAttack();
};

