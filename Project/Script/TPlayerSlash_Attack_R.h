#pragma once
#include <Engine/CState.h>

class CPlayerScript;
class TPlayerSlash_Attack_R :
	public CState
{
private:
    CPlayerScript* m_Script;
    bool m_IsLeftSlash;
private:
    void update() override;

    void Enter() override;
    void Exit() override;

public:
    TPlayerSlash_Attack_R();
    ~TPlayerSlash_Attack_R();
};

