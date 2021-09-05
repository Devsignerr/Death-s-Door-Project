#pragma once
#include <Engine/CState.h>

class CTexture;
class CIronmaceScript;
class TIronDeath :
    public CState
{
private:
    float            m_PaperBurnTime;
    CIronmaceScript* m_Script;
    Ptr<CTexture>    m_RedTex;
    Ptr<CTexture>    m_PaperBurnTex;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TIronDeath();
    ~TIronDeath();
};

