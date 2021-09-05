#pragma once
#include <Engine/CState.h>

class CTexture;
class CCastleScript;
class TCastleDeath : public CState
{
private:
    float            m_Death;
    float            m_PaperBurnTime;
    CCastleScript* m_Script;
    Ptr<CTexture>    m_RedTex;
    Ptr<CTexture>    m_PaperBurnTex;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleDeath();
    ~TCastleDeath();
};

