#pragma once
#include <Engine/CState.h>

class TCastleDeath : public CState
{
private:
    float m_PaperBurnTime;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TCastleDeath();
    ~TCastleDeath();
};

