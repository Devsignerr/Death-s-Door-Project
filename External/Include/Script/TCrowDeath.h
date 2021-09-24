#pragma once
#include <Engine/CState.h>

class CTexture;
class CCrowScript;
class TCrowDeath :
    public CState
{
private:
    float           m_PaperBurnTime;
    CCrowScript* m_Script;
    Ptr<CTexture>   m_RedTex;
    Ptr<CTexture>   m_PaperBurnTex;
    bool            m_bDead;
private:
    void update() override;
    void CreateDoor();

    void Enter() override;
    void Exit() override;

public:
    TCrowDeath();
    ~TCrowDeath();
};

