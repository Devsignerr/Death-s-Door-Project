#include "pch.h"
#include "CCrowScript.h"
#include "CCrowBullet.h"
#include "CCrowBatBullet.h"
#include "CCrowEggBullet.h"
#include "CCrowBullet.h"
#include "CPlayerScript.h"


#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCollider3D.h>

#pragma region CrowStateHeader

#include "TCrowBatBullet.h"
#include "TCrowCutScene.h"
#include "TCrowDeath.h"
#include "TCrowGuidedBullet.h"
#include "TCrowIdle.h"
#include "TCrowJump.h"
#include "TCrowLeftSpin.h"
#include "TCrowRightSpin.h"
#include "TCrowSliding.h"
#include "TCrowSlidingReady.h"
#include "TCrowSpitting.h"
#include "TCrowStomp.h"
#include "TCrowWalk.h"
#include "TCrowRun.h"
#include "TCrowStandingDeath.h"
#include "TCrowSlidingRot.h"

#pragma endregion
bool CCrowScript::CrowDeathCheck = false;

void CCrowScript::CreateChains()
{
    if (m_queueChain.size() >= m_iChainCount)
        return;

    for (int i = 0; i < m_iChainCount; ++i)
    {
        wstring PrefabName = L"CrowChain";

        Ptr<CPrefab> Prefab = CResMgr::GetInst()->FindRes<CPrefab>(PrefabName);

        if (nullptr == Prefab)
        {
            wstring PrefabPath = L"prefab\\" + PrefabName + L".pref";
            Ptr<CPrefab> Prefab = CResMgr::GetInst()->Load<CPrefab>(PrefabName, PrefabPath);
        }

        CGameObject* pGameObject = Prefab->Instantiate();
        pGameObject->awake();

        int PrefabCount = i;

        wchar_t Str[10] = {};
        _itow_s(PrefabCount, Str, 10);
        wstring PrefabNumber = wstring(Str);

        pGameObject->SetName(PrefabName + PrefabNumber);

        pGameObject->SetAllMeshrenderActive(false);
        pGameObject->SetAllColliderActive(false);

        m_queueChain.push(pGameObject);

        CSceneMgr::GetInst()->GetCurScene()->AddObject(pGameObject, (UINT)LAYER_TYPE::BOSS_EFFECT);

    }
}

CGameObject* CCrowScript::GetChain()
{
    if (!m_queueChain.empty())
    {
        CGameObject* pObj = m_queueChain.front();

        pObj->SetAllMeshrenderActive(true);
        pObj->SetAllColliderActive(true);

        m_queueChain.pop();

        return pObj;
    }
    return nullptr;
}


void CCrowScript::ReturnChain(CGameObject* _Obj)
{
    _Obj->SetAllMeshrenderActive(false);
    _Obj->SetAllColliderActive(false);

    m_queueChain.push(_Obj);
}


void CCrowScript::ChangeState(CROW_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
    m_eState = _eState;

    wstring StateName = {};

    //STATE 따라 이름 바꿔줘야 함 
    map<CROW_STATE, wstring>::iterator iter = m_mapState.find(_eState);

    if (m_mapState.end() != iter)
        StateName = iter->second;
    else
        assert(0);

    m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}

void CCrowScript::CreateCrowBullet(Vec3 _Pos, Vec3 _Dir)
{
    CGameObject* Obj = nullptr;
    CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

    Obj = IntanciatePrefab(L"CrowHead", (UINT)LAYER_TYPE::BOSS_EFFECT);
    Obj->Transform()->SetLocalPos(_Pos);
    Obj->GetChild()[1]->ParticleSystem()->SetPaperburnPTC(true);

    CurScene->AddObject(Obj, (UINT)LAYER_TYPE::BOSS_EFFECT);

    CCrowBullet* Script = (CCrowBullet*)Obj->GetScript();
    Script->SetAwakeMoveDir(_Dir);
    Script->SetActive(true);

    CGameObject* Col = new CGameObject;
    Col->SetName(L"CrowHead_Col");

    Col->AddComponent(new CTransform);
    Col->AddComponent(new CMeshRender);
    Col->AddComponent(new CCollider3D);

    Col->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
    Col->Transform()->SetLocalScale(Vec3(200.f, 200.f, 600.f));

    Col->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
    Col->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

    CurScene->AddObject(Col, (UINT)LAYER_TYPE::BOSS_BULLET_COL);

    Obj->AddChild(Col);

    Obj->awake();
}

void CCrowScript::awake()
{
    CBossScript::awake();

    //CreateChains();

    CreateCol(L"CrowBossCol", Vec3(0.0f, 50.0f, 50.0f), Vec3(130.0f, 300.0f, 200.0f), LAYER_TYPE::BOSS_COL);
    CreateCol(L"CrowBossAttackCol", Vec3(0.0f, 150.0f, 50.0f), Vec3(100.0f, 100.0f, 100.0f), LAYER_TYPE::BOSS_ATTACK_COL);

    if (nullptr != m_pFSM)
        return;

    m_pFSM = new CFSM;
    m_pFSM->SetObject(GetObj());

    TCrowCutScene* CutSceneState = new TCrowCutScene;                     //여기에서 스테이트 생성 , FSM에 추가시킴 
    m_pFSM->AddState(L"CutScene", CutSceneState);                        // FSM에 상태를 추가한다 
    m_mapState.insert(make_pair(CROW_STATE::CUTSCENE, L"CutScene"));         //내 상태와 , 상태이름을 맵으로 저장한다 

    TCrowRun* RunState = new TCrowRun;
    m_pFSM->AddState(L"Run", RunState);
    m_mapState.insert(make_pair(CROW_STATE::RUN, L"Run"));

    TCrowStandingDeath* StandingDeathState = new TCrowStandingDeath;
    m_pFSM->AddState(L"StandingDeath", StandingDeathState);
    m_mapState.insert(make_pair(CROW_STATE::STANDINGDEATH, L"StandingDeath"));

    TCrowWalk* WalkState = new TCrowWalk;
    m_pFSM->AddState(L"Walk", WalkState);
    m_mapState.insert(make_pair(CROW_STATE::WALK, L"Walk"));

    TCrowStomp* StompState = new TCrowStomp;
    m_pFSM->AddState(L"Stomp", StompState);
    m_mapState.insert(make_pair(CROW_STATE::STOMP, L"Stomp"));

    TCrowSpitting* SpittingState = new TCrowSpitting;
    m_pFSM->AddState(L"Spitting", SpittingState);
    m_mapState.insert(make_pair(CROW_STATE::SPITTING, L"Spitting"));

    TCrowSlidingReady* SlidingReadyState = new TCrowSlidingReady;
    m_pFSM->AddState(L"SlidingReady", SlidingReadyState);
    m_mapState.insert(make_pair(CROW_STATE::SLIDINGREADY, L"SlidingReady"));

    TCrowSliding* SlidingState = new TCrowSliding;
    m_pFSM->AddState(L"Sliding", SlidingState);
    m_mapState.insert(make_pair(CROW_STATE::SLIDING, L"Sliding"));

    TCrowRightSpin* RightSpinState = new TCrowRightSpin;
    m_pFSM->AddState(L"RightSpin", RightSpinState);
    m_mapState.insert(make_pair(CROW_STATE::RIGHTSPIN, L"RightSpin"));

    TCrowLeftSpin* LeftSpinState = new TCrowLeftSpin;
    m_pFSM->AddState(L"LeftSpin", LeftSpinState);
    m_mapState.insert(make_pair(CROW_STATE::LEFTSPIN, L"LeftSpin"));

    TCrowJump* JumpState = new TCrowJump;
    m_pFSM->AddState(L"Jump", JumpState);
    m_mapState.insert(make_pair(CROW_STATE::JUMP, L"Jump"));

    TCrowIdle* IdleState = new TCrowIdle;
    m_pFSM->AddState(L"Idle", IdleState);
    m_mapState.insert(make_pair(CROW_STATE::IDLE, L"Idle"));

    TCrowGuidedBullet* GuidedBulletState = new TCrowGuidedBullet;
    m_pFSM->AddState(L"GuidedBullet", GuidedBulletState);
    m_mapState.insert(make_pair(CROW_STATE::GUIDEDBULLET, L"GuidedBullet"));

    TCrowDeath* DeathState = new TCrowDeath;
    m_pFSM->AddState(L"Death", DeathState);
    m_mapState.insert(make_pair(CROW_STATE::DEATH, L"Death"));

    TCrowBatBullet* BatBullet = new TCrowBatBullet;
    m_pFSM->AddState(L"BatBullet", BatBullet);
    m_mapState.insert(make_pair(CROW_STATE::BATBULLET, L"BatBullet"));

    ChangeState(CROW_STATE::IDLE, 0.04f, L"Idle");
}

void CCrowScript::update()
{
    CBossScript::update();
    m_pFSM->update();
}

void CCrowScript::OnCollisionEnter(CGameObject* _pOther)
{
    CGameObject* Obj = _pOther;

    if (m_Hp <= 0)
        return;

    if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
    {
        CreateCrowBullet(Transform()->GetLocalPos(), CPlayerScript::GetPlayerFront());

        --m_Hp;

        if (0 == m_Hp)
        {

            m_pFSM->ChangeState(L"Death", 0.04f, L"Death", true);
        }

        else
        {
            CActorScript::OnCollisionEnter(_pOther);
        }
    }
}

void CCrowScript::OnCollision(CGameObject* _pOther)
{
    CGameObject* Obj = _pOther;

    if ((UINT)LAYER_TYPE::INDETERMINATE == Obj->GetLayerIndex())
    {
        const vector<CGameObject*>& Temp = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::INDETERMINATE)->GetObjects();

        CCrowEggBullet* Script = nullptr;

        for (size_t i = 0; i < Temp.size(); ++i)
        {
            Script = (CCrowEggBullet*)Temp[i]->GetScript();
            if (Script)
            {
                if (false == Script->GetHead())
                {
                    Script->FindToDeadCheck(Obj);
                    break;
                }
            }
        }
    }
}

void CCrowScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CCrowScript::SaveToScene(FILE* _pFile)
{
    CBossScript::SaveToScene(_pFile);
}

void CCrowScript::LoadFromScene(FILE* _pFile)
{
    CBossScript::LoadFromScene(_pFile);
}

CCrowScript::CCrowScript()
    : m_pFSM(nullptr)
    , m_eState(CROW_STATE::END)
    , m_Hp(15)
    //, m_iChainCount(50)
{
    m_iScriptType = (int)SCRIPT_TYPE::CROWSCRIPT;
}

CCrowScript::~CCrowScript()
{
    SAFE_DELETE(m_pFSM);
}