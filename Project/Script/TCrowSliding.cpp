#include "pch.h"
#include "TCrowSliding.h"
#include "TCrowSlidingReady.h"
#include "CCrowScript.h"
#include "CRandomMgrScript.h"
#include "CPlayerScript.h"
#include "CCrowEggBullet.h"


#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>

void TCrowSliding::SlidingPosSet(int _Point)
{
    if (m_PrevPoint - 3 <= _Point && m_PrevPoint + 3 >= _Point)
        _Point += 5;


    UINT  iSliceCount = 20;
    float Interval = XM_2PI / iSliceCount;
    float fTheta = Interval * _Point;

    // 중심 점 -> 플레이어의 위치
    m_Point = CPlayerScript::GetPlayerPos();

    // 테두리 점 -> 슬라이딩 시작 위치
    Vec3 SlidingPoint = Vec3(m_Radius * cosf(fTheta), 0.0f, m_Radius * sinf(fTheta));

    // 정해진 위치로 이동
    m_Point += SlidingPoint;
    GetObj()->Transform()->SetLocalPos(m_Point);

    // 회전
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
    Vec3 Pos = GetObj()->Transform()->GetLocalPos();
    Vec3 relativePos = PlayerPos - Pos;
    Vec3 Rot = GetObj()->Transform()->GetLocalRot();
    Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vCross = relativePos.Cross(vFront);
    Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    float dot = vCross.Dot(vUP);
    relativePos.Normalize();

    float RotAngle = vFront.Dot(relativePos);
    RotAngle = acos(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot > 0.0)
        Rot.y -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        Rot.y += RotAngle;

    GetObj()->Transform()->SetLocalRot(Rot);

    // 진행 방향
    m_SlidingDir = relativePos;
    m_PrevPoint = _Point;
}

void TCrowSliding::DiagonalPattern(int _StartPoint)
{
    UINT  iSliceCount = 20;
    float Interval = XM_2PI / iSliceCount;
    float fTheta = Interval * _StartPoint;

    // 중심 점 -> 플레이어의 위치
    m_Point = CPlayerScript::GetPlayerPos();
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

    if (3 == _StartPoint)
    {
        if (2 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x -= 400.0f;
            fTheta = Interval * (_StartPoint + 10);
        }
        else if (3 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x += 400.0f;
        }
        else if (4 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x -= 800.0f;
            fTheta = Interval * (_StartPoint + 10);
        }
        else if (5 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x += 800.0f;
        }
    }
    else
    {
        if (2 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x += 400.0f;
            fTheta = Interval * (_StartPoint - 10);
        }
        else if (3 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x -= 400.0f;

        }
        else if (4 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x += 800.0f;
            fTheta = Interval * (_StartPoint - 10);
        }
        else if (5 == m_SlidingCount)
        {
            m_Point = CPlayerScript::GetPlayerPos();
            m_Point.x -= 800.0f;
        }
    }

    PlayerPos = m_Point;

    // 테두리 점 -> 슬라이딩 시작 위치
    Vec3 SlidingPoint = Vec3(m_Radius * cosf(fTheta), 0.0f, m_Radius * sinf(fTheta));

    // 정해진 위치로 이동
    m_Point += SlidingPoint;
    GetObj()->Transform()->SetLocalPos(m_Point);

    // 회전
    Vec3 Pos = GetObj()->Transform()->GetLocalPos();
    Vec3 relativePos = PlayerPos - Pos;
    Vec3 Rot = GetObj()->Transform()->GetLocalRot();
    Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vCross = relativePos.Cross(vFront);
    Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    float dot = vCross.Dot(vUP);
    relativePos.Normalize();

    float RotAngle = vFront.Dot(relativePos);
    RotAngle = acos(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot > 0.0)
        Rot.y -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        Rot.y += RotAngle;

    GetObj()->Transform()->SetLocalRot(Rot);

    // 진행 방향
    m_SlidingDir = relativePos;
}

void TCrowSliding::CrossPattern(int _StartPoint)
{
    UINT  iSliceCount = 20;
    float Interval = XM_2PI / iSliceCount;
    float fTheta = Interval * _StartPoint;

    // 중심 점 -> 플레이어의 위치
    m_Point = CPlayerScript::GetPlayerPos();
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

    if (0 == _StartPoint)
    {
        if (2 == m_SlidingCount)
        {
            int UpDown = CRandomMgrScript::GetRandomintNumber(0, 1);

            if (0 == UpDown)
                fTheta = Interval * (_StartPoint + 5);
            else
                fTheta = Interval * (_StartPoint + 15);
        }
    }
    else
    {
        if (2 == m_SlidingCount)
        {
            int UpDown = CRandomMgrScript::GetRandomintNumber(0, 1);

            if (0 == UpDown)
                fTheta = Interval * (_StartPoint - 5);
            else
                fTheta = Interval * (_StartPoint + 5);
        }
    }

    PlayerPos = m_Point;

    // 테두리 점 -> 슬라이딩 시작 위치
    Vec3 SlidingPoint = Vec3(m_Radius * cosf(fTheta), 0.0f, m_Radius * sinf(fTheta));

    // 정해진 위치로 이동
    m_Point += SlidingPoint;
    GetObj()->Transform()->SetLocalPos(m_Point);

    // 회전
    Vec3 Pos = GetObj()->Transform()->GetLocalPos();
    Vec3 relativePos = PlayerPos - Pos;
    Vec3 Rot = GetObj()->Transform()->GetLocalRot();
    Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vCross = relativePos.Cross(vFront);
    Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    float dot = vCross.Dot(vUP);
    relativePos.Normalize();

    float RotAngle = vFront.Dot(relativePos);
    RotAngle = acos(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot > 0.0)
        Rot.y -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        Rot.y += RotAngle;

    GetObj()->Transform()->SetLocalRot(Rot);

    // 진행 방향
    m_SlidingDir = relativePos;
}

void TCrowSliding::GoboardPattern(int _StartPoint)
{
    UINT  iSliceCount = 20;
    float Interval = XM_2PI / iSliceCount;
    float fTheta = Interval * _StartPoint;

    // 중심 점 -> 플레이어의 위치
    m_Point = CPlayerScript::GetPlayerPos();
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

    if (3 == _StartPoint)
    {
        m_Point = CPlayerScript::GetPlayerPos();
        m_Point.x += 400.0f;
    }
    else if (18 == _StartPoint)
    {
        m_Point = CPlayerScript::GetPlayerPos();
        m_Point.x += 400.0f;
    }
    else if (8 == _StartPoint)
    {
        m_Point = CPlayerScript::GetPlayerPos();
        m_Point.x -= 400.0f;
    }
    else if (13 == _StartPoint)
    {
        m_Point = CPlayerScript::GetPlayerPos();
        m_Point.x -= 400.0f;
    }

    PlayerPos = m_Point;

    // 테두리 점 -> 슬라이딩 시작 위치
    Vec3 SlidingPoint = Vec3(m_Radius * cosf(fTheta), 0.0f, m_Radius * sinf(fTheta));

    // 정해진 위치로 이동
    m_Point += SlidingPoint;
    GetObj()->Transform()->SetLocalPos(m_Point);

    // 회전
    Vec3 Pos = GetObj()->Transform()->GetLocalPos();
    Vec3 relativePos = PlayerPos - Pos;
    Vec3 Rot = GetObj()->Transform()->GetLocalRot();
    Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vCross = relativePos.Cross(vFront);
    Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    float dot = vCross.Dot(vUP);
    relativePos.Normalize();

    float RotAngle = vFront.Dot(relativePos);
    RotAngle = acos(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot > 0.0)
        Rot.y -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        Rot.y += RotAngle;

    GetObj()->Transform()->SetLocalRot(Rot);

    // 진행 방향
    m_SlidingDir = relativePos;
}

void TCrowSliding::EndMotion()
{
    UINT  iSliceCount = 20;
    float Interval = XM_2PI / iSliceCount;

    // 회전
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
    Vec3 Pos = GetObj()->Transform()->GetLocalPos();
    Vec3 relativePos = PlayerPos - Pos;
    Vec3 Rot = GetObj()->Transform()->GetLocalRot();
    Vec3 vFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
    Vec3 vCross = relativePos.Cross(vFront);
    Vec3 vUP = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    float dot = vCross.Dot(vUP);
    relativePos.Normalize();

    float RotAngle = vFront.Dot(relativePos);
    RotAngle = acos(RotAngle);

    //플레이어는 내 왼쪽에 있다 
    if (dot > 0.0)
        Rot.y -= RotAngle;

    //플레이어는 내 오른쪽에 있다 
    else if (dot < 0.0)
        Rot.y += RotAngle;

    GetObj()->Transform()->SetLocalRot(Rot);
}


void TCrowSliding::CreateChain()
{

    const vector<CGameObject*>& Temp = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::INDETERMINATE)->GetObjects();

    if (true == Temp.empty())
    {
        Vec3 Pos = GetObj()->Transform()->GetLocalPos();
        Pos.y += 150.0f;

        CGameObject* Obj = new CGameObject;
        Obj->AddComponent(new CTransform);
        Obj->AddComponent(new CCrowEggBullet);
        Obj->Transform()->SetLocalPos(m_Point);

        CCrowEggBullet* Script = (CCrowEggBullet*)Obj->GetScript();
        Script->SetStartPos(Pos);
        Script->SetDir(m_SlidingDir);
        Script->SetCrowBossRot(GetObj()->Transform()->GetLocalRot());

        float Range = m_PlayerToBossDistance + 4000.0f;
        Script->SetRange(Range);

        Script->awake();

        CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
        CurScene->AddObject(Obj, (UINT)LAYER_TYPE::INDETERMINATE);
    }

    m_PlayerPos = CPlayerScript::GetPlayerPos();
}

void TCrowSliding::ChainMoveEndCheck()
{
    const vector<CGameObject*>& Temp = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::INDETERMINATE)->GetObjects();

    if (!Temp.empty())
    {
        CCrowEggBullet* Script = nullptr;

        for (size_t i = 0; i < Temp.size(); ++i)
        {
            Script = (CCrowEggBullet*)Temp[i]->GetScript();
            if (Script)
            {
                if (false == Script->GetHead())
                {
                    m_ChainMoveEnd = true;
                    break;
                }
            }
        }
    }

}

bool TCrowSliding::EmptyChainCheck()
{
    const vector<CGameObject*>& Temp = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::INDETERMINATE)->GetObjects();

    if (!Temp.empty())
        return true;
    else
        return false;
}

void TCrowSliding::update()
{
    CAnimator3D* CurAni = GetObj()->Animator3D();
    UINT iCurClipIdx = CurAni->GetClipIdx();

    Vec3 PlayerFront = CPlayerScript::GetPlayerFront();
    Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
    Vec3 Pos = GetObj()->Transform()->GetLocalPos();

    m_PlayerToBossDistance = Vec3::Distance(m_PlayerPos, Pos);

    if (true == EmptyChainCheck() && true == m_ChainMoveEnd)
    {
        Pos += CTimeMgr::GetInst()->GetfDT() * m_SlidingDir * 10000.0f;
        GetObj()->Transform()->SetLocalPos(Pos);
    }
    else
    {
        if (true == m_ChainMoveEnd)
        {
            ++m_SlidingCount;
            m_ChainMoveEnd = false;

            if (m_SlidingTotalCount >= m_SlidingCount)
            {
                //ClearChain();
                if (true == m_SpecialPattern)
                {
                    if (0 == m_SpecialPatternType)
                    {
                        // 사선
                        int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

                        if (0 == Pattern)
                            DiagonalPattern(3); // 위쪽에서 아래쪽
                        else
                            DiagonalPattern(13); // 아래쪽에서 위쪽
                    }
                    else if (1 == m_SpecialPatternType)
                    {
                        //십자
                        int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

                        if (0 == Pattern)
                            CrossPattern(0); // 오른쪽에서 왼쪽
                        else
                            CrossPattern(10);  // 왼쪽에서 오른쪽
                    }
                    else if (2 == m_SpecialPatternType)
                    {
                        // 바둑판
                        if (1 == m_SlidingCount)
                            GoboardPattern(18);
                        else if (2 == m_SlidingCount)
                            GoboardPattern(3);
                        else if (3 == m_SlidingCount)
                            GoboardPattern(8);
                        else if (4 == m_SlidingCount)
                            GoboardPattern(13);
                    }
                }
                else
                {
                    int Point = CRandomMgrScript::GetRandomintNumber(1, 20);
                    SlidingPosSet(Point);
                }

                CreateChain();
            }
            else
            {
                EndMotion();
                GetFSM()->ChangeState(L"Jump", 0.01f, L"Jump", false);
            }
        }
        ChainMoveEndCheck();
    }

}

void TCrowSliding::Enter()
{
    m_ChainMoveEnd = true;
    m_PlayerPos = CPlayerScript::GetPlayerPos();
    m_IsSliding = true;
    m_Script = (CCrowScript*)GetScript();
    TCrowSlidingReady* LastDir = (TCrowSlidingReady*)GetFSM()->FindState(L"SlidingReady");
    m_SlidingDir = LastDir->GetLastDir();
    m_SpecialPattern = CRandomMgrScript::GetRandomintNumber(0, 1);
    m_SpecialPatternType = CRandomMgrScript::GetRandomintNumber(0, 2);

    if (true == m_SpecialPattern)
    {
        // 사선
        if (0 == m_SpecialPatternType)
            m_SlidingTotalCount = CRandomMgrScript::GetRandomintNumber(2, 4);
        // 십자
        else if (1 == m_SpecialPatternType)
            m_SlidingTotalCount = 2;
        // 바둑판
        else if (2 == m_SpecialPatternType)
            m_SlidingTotalCount = 4;
    }
    else
    {
        m_SlidingTotalCount = 3;
    }

    m_Script->OnOffAttackCol(true);

}

void TCrowSliding::Exit()
{
    m_SlidingDir = {};
    m_SlidingCount = 0;
    m_SpecialPattern = 0;
    m_SpecialPatternType = -1;
    m_SpecialPatternEndCheck = false;
    m_SlidingTime = 0.0f;
    m_PrevPoint = 0;
    m_Script->OnOffAttackCol(false);
}

TCrowSliding::TCrowSliding()
    : m_SpecialPattern(false)
    , m_SlidingCount(0)
    , m_SlidingDir{}
    , m_SpecialPatternType(-1)
    , m_SpecialPatternEndCheck(false)
    , m_Script(nullptr)
    , m_SlidingTime(0.0f)
    , m_PrevPoint(0)
    , m_IsSliding(false)
    , m_Radius(3500.0f)
    , m_PlayerToBossDistance(0.0f)
    , m_ChainMoveEnd(false)
    , m_SlidingTotalCount(0)
{
}

TCrowSliding::~TCrowSliding()
{
}