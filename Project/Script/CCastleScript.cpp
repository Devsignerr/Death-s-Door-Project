#include "pch.h"
#include "CCastleScript.h"
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CState.h>
#include <Engine/CCollider3D.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CLayer.h>

#include "CPlayerScript.h"
#include "CRandomMgrScript.h"

#pragma region CastleStateHeader
#include "TCastleCutScene.h"
#include "TCastleFly.h"
#include "TCastleFly_Finish.h"
#include "TCastleFly_Start.h"
#include "TCastleFly_Stay.h"
#include "TCastleLaser_Finish.h"
#include "TCastleLaser_Ready.h"
#include "TCastleLeftBack_HalfSpin.h"
#include "TCastleLeftFront_HalfSpin.h"
#include "TCastleMissile_Close.h"
#include "TCastleMissile_Open.h"
#include "TCastleRightBack_HalfSpin.h"
#include "TCastleRightFront_HalfSpin.h"
#include "TCastleSpin.h"
#include "TCastleWalk.h"
#include "TCastleDeath.h"
#pragma endregion


void CCastleScript::ChangeState(CASTLE_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	m_eState = _eState;

	wstring StateName = {};

	//STATE 따라 이름 바꿔줘야 함 
	map<CASTLE_STATE, wstring>::iterator iter = m_mapState.find(_eState);

	if (m_mapState.end() != iter)
		StateName = iter->second;
	else
		assert(0);

	m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}

void CCastleScript::CreateLaserPoint()
{
	CGameObject* Obj = new CGameObject;
	Obj->SetName(L"LaserPoint");

	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCollider3D);

	Obj->Transform()->SetLocalPos(Vec3(3560.0f, -4073.0f, 1113.0f));
	Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, (UINT)LAYER_TYPE::INDETERMINATE);
}

void CCastleScript::awake()
{
	CBossScript::awake();
	CreateLaserPoint();
	CreateCol(L"CastleCol", Vec3(0.0f, -50000.0f, 0.0f), Vec3(140000.0f, 100000.0f, 40000.0f), LAYER_TYPE::BOSS_COL);
	CreateCol(L"CastleAttackCol", Vec3(60000.0f, -10000.0f, 25000.0f), Vec3(50000.0f, 30000.0f, 50000.0f), LAYER_TYPE::BOSS_ATTACK_COL);

	if (nullptr != m_pFSM)
		return;

	m_pFSM = new CFSM;
	m_pFSM->SetObject(GetObj());

	TCastleCutScene* CutSceneState = new TCastleCutScene;						//여기에서 스테이트 생성 , FSM에 추가시킴 
	m_pFSM->AddState(L"CutScene", CutSceneState);								// FSM에 상태를 추가한다 
	m_mapState.insert(make_pair(CASTLE_STATE::CUTSCENE, L"CutScene"));			//내 상태와 , 상태이름을 맵으로 저장한다 

	TCastleFly* FlyState = new TCastleFly;
	m_pFSM->AddState(L"Fly", FlyState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY, L"Fly"));

	TCastleFly_Finish* FlyFinishState = new TCastleFly_Finish;
	m_pFSM->AddState(L"Fly_Finish", FlyFinishState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY_FINISH, L"Fly_Finish"));

	TCastleFly_Start* FlyStartState = new TCastleFly_Start;
	m_pFSM->AddState(L"Fly_Start", FlyStartState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY_START, L"Fly_Start"));

	TCastleFly_Stay* FlyStayState = new TCastleFly_Stay;
	m_pFSM->AddState(L"Fly_Stay", FlyStayState);
	m_mapState.insert(make_pair(CASTLE_STATE::FLY_STAY, L"Fly_Stay"));

	TCastleLaser_Finish* LaserFinishState = new TCastleLaser_Finish;
	m_pFSM->AddState(L"Laser_Finish", LaserFinishState);
	m_mapState.insert(make_pair(CASTLE_STATE::LASER_FINISH, L"Laser_Finish"));

	TCastleLaser_Ready* LaserReadyState = new TCastleLaser_Ready;
	m_pFSM->AddState(L"Laser_Ready", LaserReadyState);
	m_mapState.insert(make_pair(CASTLE_STATE::LASER_READY, L"Laser_Ready"));

	TCastleLeftBack_HalfSpin* LaftBackHalfSpinState = new TCastleLeftBack_HalfSpin;
	m_pFSM->AddState(L"LeftBack_HalfSpin", LaftBackHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::LEFTBACK_HALFSPIN, L"LeftBack_HalfSpin"));

	TCastleLeftFront_HalfSpin* LaftFrontHalfSpinState = new TCastleLeftFront_HalfSpin;
	m_pFSM->AddState(L"LeftFront_HalfSpin", LaftFrontHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::LEFTFRONT_HALFSPIN, L"LeftFront_HalfSpin"));

	TCastleMissile_Close* MissileCloseState = new TCastleMissile_Close;
	m_pFSM->AddState(L"Missile_Close", MissileCloseState);
	m_mapState.insert(make_pair(CASTLE_STATE::MISSILE_CLOSE, L"Missile_Close"));

	TCastleMissile_Open* MissileOpenState = new TCastleMissile_Open;
	m_pFSM->AddState(L"Missile_Open", MissileOpenState);
	m_mapState.insert(make_pair(CASTLE_STATE::MISSILE_OPEN, L"Missile_Open"));

	TCastleRightBack_HalfSpin* RightBackHalfSpinState = new TCastleRightBack_HalfSpin;
	m_pFSM->AddState(L"RightBack_HalfSpin", RightBackHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::RIGHTBACK_HALFSPIN, L"RightBack_HalfSpin"));

	TCastleRightFront_HalfSpin* RightFrontHalfSpinState = new TCastleRightFront_HalfSpin;
	m_pFSM->AddState(L"RightFront_HalfSpin", RightFrontHalfSpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::RIGHTFRONT_HALFSPIN, L"RightFront_HalfSpin"));

	TCastleSpin* SpinState = new TCastleSpin;
	m_pFSM->AddState(L"Spin", SpinState);
	m_mapState.insert(make_pair(CASTLE_STATE::SPIN, L"Spin"));

	TCastleWalk* WalkState = new TCastleWalk;
	m_pFSM->AddState(L"Walk", WalkState);
	m_mapState.insert(make_pair(CASTLE_STATE::WALK, L"Walk"));

	TCastleDeath* DeathState = new TCastleDeath;
	m_pFSM->AddState(L"Death", DeathState);
	m_mapState.insert(make_pair(CASTLE_STATE::DEATH, L"Death"));


	m_pFSM->ChangeState(L"CutScene", 0.04f, L"CutScene", false);
}

void CCastleScript::update()
{
	CBossScript::update();
	m_pFSM->update();
}

void CCastleScript::CheckAttackDirection()
{
	CASTLE_DIRECTION AttackDir = {};

	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = Transform()->GetLocalPos();
	Vec3 relativePos = PlayerPos - Pos;
	relativePos.y = 0.0f;
	Vec3 Rot = Transform()->GetLocalRot();

	Vec3 vFront = -Transform()->GetLocalDir(DIR_TYPE::FRONT);
	vFront.y = 0.0f;
	vFront.Normalize();
	relativePos.Normalize();
	//내가 플레이어를 바라보는 벡터와 내 front 벡터를 외적 
	Vec3 vCross = relativePos.Cross(vFront);

	//위 결과에서 나온 벡터와 몬스터의 up벡터(위에서 쓴 front 아님)를 내적했을때 , 
	// 위 값이 양수면 플레이어는 내 왼쪽 , 음수면 오른쪽에 있는 것 

	//프론트를 얻어왔지만 회전시켜서 세웠으므로 업벡터로 쓰임 
	Vec3 vUP = Transform()->GetLocalDir(DIR_TYPE::UP);

	float dot = vCross.Dot(vUP);
	float dist = (-vFront + relativePos).Length();

	Vec3 relativePos2 = Pos - PlayerPos;
	relativePos2.Normalize();
	float Dot = vFront.Dot(relativePos2);
	Dot = acos(Dot);
	if (Dot > -(XM_PI / 2.0f) && Dot < (XM_PI / 2.0f))
	{
		// 앞쪽

		// 왼쪽
		if (dot > 0.0f)
			AttackDir = CASTLE_DIRECTION::FRONT_LEFT;
		// 오른쪽
		else if (dot < 0.0f)
			AttackDir = CASTLE_DIRECTION::FRONT_RIGHT;
	}
	else
	{
		// 뒤쪽 

		// 왼쪽
		if (dot > 0.0)
			AttackDir = CASTLE_DIRECTION::BACK_LEFT;
		// 오른쪽
		else if (dot < 0.0)
			AttackDir = CASTLE_DIRECTION::BACK_RIGHT;
	}

	int AllDir = CRandomMgrScript::GetRandomintNumber(0, 2);

	if (1 == AllDir)
	{
		AttackDir = CASTLE_DIRECTION::ALLDIR;
	}

	switch (AttackDir)
	{
	case CCastleScript::CASTLE_DIRECTION::FRONT_RIGHT:
		m_pFSM->ChangeState(L"RightFront_HalfSpin", 0.1f, L"RightFront_HalfSpin", false);
		break;
	case CCastleScript::CASTLE_DIRECTION::FRONT_LEFT:
		m_pFSM->ChangeState(L"LeftFront_HalfSpin", 0.1f, L"LeftFront_HalfSpin", false);
		break;
	case CCastleScript::CASTLE_DIRECTION::BACK_RIGHT:
		m_pFSM->ChangeState(L"RightBack_HalfSpin", 0.1f, L"RightBack_HalfSpin", false);
		break;
	case CCastleScript::CASTLE_DIRECTION::BACK_LEFT:
		m_pFSM->ChangeState(L"LeftBack_HalfSpin", 0.1f, L"LeftBack_HalfSpin", false);
		break;
	case CCastleScript::CASTLE_DIRECTION::ALLDIR:
		m_pFSM->ChangeState(L"Spin", 0.1f, L"Spin", false);
		break;
	default:
		break;
	}
}

void CCastleScript::PatternChoice()
{
	int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

	if (0 == Pattern)
	{
		m_pFSM->ChangeState(L"Fly_Start", 0.1f, L"Fly_Start", false);
	}
	else if (1 == Pattern)
	{
		m_pFSM->ChangeState(L"Fly", 0.1f, L"Fly", false);
	}
}

void CCastleScript::OnCollisionEnter(CGameObject* _pOther)
{
	CActorScript::OnCollisionEnter(_pOther);
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::PLAYER_ATTACK_COL == Obj->GetLayerIndex())
	{
		--m_Hp;

		if (0 == m_Hp)
			m_pFSM->ChangeState(L"Death", 0.04f, L"Death", true);
	}
}

void CCastleScript::OnCollision(CGameObject* _pOther)
{
}

void CCastleScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CCastleScript::SaveToScene(FILE* _pFile)
{
	CBossScript::SaveToScene(_pFile);
}

void CCastleScript::LoadFromScene(FILE* _pFile)
{
	CBossScript::LoadFromScene(_pFile);
}

CCastleScript::CCastleScript()
	: m_pFSM(nullptr)
	, m_eState(CASTLE_STATE::END)
	, m_Hp(15)
{
	m_iScriptType = (int)SCRIPT_TYPE::CASTLESCRIPT;
}

CCastleScript::~CCastleScript()
{
	SAFE_DELETE(m_pFSM);
}
