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
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCrowDeath();
    ~TCrowDeath();
};

