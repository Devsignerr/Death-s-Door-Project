#pragma once
#include "CProjectile.h"
class CCrowEggBullet :
    public CProjectile
{
private:
    struct SLIDINGINFO
    {
        CGameObject* Obj;
        bool         MoveCheck;
        float         MovementValue;
        bool         DeadCheck;
    };

private:
    list<SLIDINGINFO>   m_SlidingLine;
    Vec3            m_StartPos;
    float            m_Range;
    Vec3            m_Dir;
    Vec3            m_CrowBossRot;
private:
    void Create();
    void Move();
    void Dead();
    void AllDead();

    void AddSlidingLine(CGameObject* _SildingLine)
    {
        SLIDINGINFO Temp = {};
        Temp.Obj = _SildingLine;
        Temp.MoveCheck = false;
        Temp.MovementValue = 0.0f;

        m_SlidingLine.push_back(Temp);
    }

public:
    void SetStartPos(Vec3 _Pos) { m_StartPos = _Pos; }
    bool GetHead() { if (!m_SlidingLine.empty()) return m_SlidingLine.front().MoveCheck; }
    void FindToDeadCheck(CGameObject* _Obj);
    void SetRange(float _Range) { m_Range = _Range; }
    void SetDir(Vec3 _Dir) { m_Dir = _Dir; }
    void SetCrowBossRot(Vec3 _Rot) { m_CrowBossRot = _Rot; }
public:
    void awake() override;
    void update() override;

public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    CLONE(CCrowEggBullet);

public:
    CCrowEggBullet();
    ~CCrowEggBullet();
};
