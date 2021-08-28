#pragma once
#include "CActorScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

class CMonsterScript : public CActorScript
{
private:
    struct MONSTERJUMPINFO
    {
        Vec3   Pos;
        float  DestAttachTime;
        float  Time;
        float  Gravity;
        float  VelocityX;
        float  VelocityY;
        float  VelocityZ;
        float  MaxHeightTime;
        bool   bJump;
    };

protected:

    struct MONSTERINFO
    {
        UINT    Hp;
        float   Speed;
        float   RecognitionRange;
        float   DelayNextActionTime;

    };

    enum class MONSTERSTATE
    {
        IDLE,
        MOVE,
        CHASE,
        READY_ACTION,
        ATTACK,
        FINISH_ACTION,
        JUMP,
        SPECIAL_ACTION,
        DEATH,
        END
    };
    MONSTERINFO  m_MonsterInfo;
    MONSTERSTATE m_CurState;
    MONSTERJUMPINFO m_MonsterJumpInfo;
    float m_PaperBurnTime;



protected:
    virtual void MeleeAttack() {};
    virtual void LongDistanceAttack() {};
    virtual bool RangeSearch(float _Range);
    virtual void ChangeState(MONSTERSTATE _State, float _BlendingTime = 0.2f, const wstring& _AniName = L"", bool _Stay = false);
    bool MonsterRotateSystem(float _fRotSpeed);
    void SetMonsterJumpInfo(float _JumpTime, float _JumpHeight);
    void MonsterJumpSystem();

    virtual void OnOffAttackCol(bool _OnOff, LAYER_TYPE _Type = LAYER_TYPE::MONSTER_ATTACK_COL);
    virtual void CreateCol(const wstring& _Name, Vec3 _Pos, Vec3 _Scale, LAYER_TYPE _Type);
    virtual void TransColPos(Vec3 _Pos, LAYER_TYPE _Type = LAYER_TYPE::MONSTER_ATTACK_COL);
    virtual void TransColScale(Vec3 _Scale, LAYER_TYPE _Type = LAYER_TYPE::MONSTER_ATTACK_COL);

protected:
    virtual void Idle() {};
    virtual void Move() {};
    virtual void Chase() {};
    virtual void ReadyAction() {};
    virtual void Attack() {};
    virtual void FinishAction() {};
    virtual void SpecialAction() {};
    virtual void Jump() {};
    virtual void Death() {};



public:
    void awake() override;
    void update()override;

    void OnCollisionEnter(CGameObject* _pOther) override {};
    void OnCollision(CGameObject* _pOther) override {};
    void OnCollisionExit(CGameObject* _pOther) override {};


    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLONE(CMonsterScript);

public:
    CMonsterScript();
    virtual ~CMonsterScript();
};