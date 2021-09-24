#pragma once
#include <Engine/CState.h>

class CCastleScript;
class TCastleCutSceneWalk :
	public CState
{
private:
    CCastleScript* m_Script;
    bool           m_CurSceneCameraCheck;
    float          m_fSoundTimer;
    int            m_iRunSoundChange;
    int            m_NextCutSceneWalkCount;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleCutSceneWalk();
    ~TCastleCutSceneWalk();
};

