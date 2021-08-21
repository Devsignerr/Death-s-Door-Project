#pragma once
#include <Engine/CState.h>

class TPlayerBomb_Finish :
	public CState
{
private:
    bool            m_BulletLimit;
private:
    void Attack();
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerBomb_Finish();
    ~TPlayerBomb_Finish();
};

