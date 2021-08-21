#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerMagic : public CState
{
private:
    CPlayerScript* m_Script;
    bool           m_BulletLimit;
private:
    void Attack();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerMagic();
    ~TPlayerMagic();
};

