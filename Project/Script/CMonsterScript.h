#pragma once

#include "CActorScript.h"

class CMonsterScript : public CActorScript
{
protected:

    struct MONSTERINFO
    {
        UINT    Hp;
        float   Speed;
    };
    enum class MONSTERSTATE
    {
        IDLE,
        MOVE,
        CHASE,
        READY_ACTION,
        ATTACK,
        FINISH_ACTION,
        DEATH,
        END
    };
    MONSTERINFO  m_MonsterInfo;
    MONSTERSTATE m_CurState;
protected:
    virtual void MeleeAttack() {};
    virtual void LongDistanceAttack() {};
    virtual bool RangeSearch(float _Range);
    virtual void ChangeState(MONSTERSTATE _State, float _BlendingTime = 0.2f);

protected:
    virtual void Idle() {};
    virtual void Move() {};
    virtual void Chase() {};
    virtual void ReadyAction() {};
    virtual void Attack() {};
    virtual void FinishAction() {};
    virtual void Death() {};


public:
	virtual void update();

	virtual void OnCollisionEnter(CGameObject* _pOther);
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	~CMonsterScript();
};