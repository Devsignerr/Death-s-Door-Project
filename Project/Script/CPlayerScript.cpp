#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CCamera.h>
#include <Engine/CFSM.h>

#include <Script/CCameraScript.h>
#include <Script/CFadeScript.h>

#pragma region PlayerStateHeader
#include "TPlayerIdle.h"
#include "TPlayerRoll.h"
#include "TPlayerRun.h"
#include "TPlayerRollSlash.h"
#include "TPlayerSlash_Attack_L.h"
#include "TPlayerSlash_Attack_R.h"
#include "TPlayerCharge_L.h"
#include "TPlayerCharge_R.h"
#include "TPlayerChargeMax_L.h"
#include "TPlayerChargeMax_R.h"
#include "TPlayerCharge_Attack_L.h"
#include "TPlayerCharge_Attack_R.h"
#include "TPlayerArrow.h"
#include "TPlayerMagic.h"
#include "TPlayerBomb.h"
#include "TPlayerBomb_Finish.h"
#include "TPlayerHook.h"
#include "TPlayerHookFly.h"
#include "TPlayerGet_Item.h"
#include "TPlayerHit_Back.h"
#include "TPlayerHit_Recover.h"
#include "TPlayerDance.h"
#include "TPlayerDrown.h"
#include "TPlayerLadder_Down.h"
#include "TPlayerLadder_Top.h"
#include "TPlayerLadder_Up.h"
#pragma endregion

CPlayerScript* CPlayerScript::Player = {};
Vec3 CPlayerScript::PlayerMovePos = {};
Vec3 CPlayerScript::PlayerPrePos = {};
Vec3 CPlayerScript::PlayerPos = {};
Vec3 CPlayerScript::vPlayerRot = {};
Vec3 CPlayerScript::vPlayerFront = {};
Vec3 CPlayerScript::vPlayerUp = {};


CPlayerScript::CPlayerScript():
	m_eState(PLAYER_STATE::IDLE),
	m_fRotationSpeed(10.f)
	, m_PlayerProjectileType(PLAYER_PROJECTILE_TYPE::ARROW)
{
	Player = this;
	m_iScriptType = (UINT)SCRIPT_TYPE::PLAYERSCRIPT;
}

CPlayerScript::~CPlayerScript()
{
	// 콜라이더 달기 - 
	// 전투 다듬기 
	// 애니메이션 파티클
	// 네비 메쉬 달아주기
	// 네비메쉬 편집

	// 블룸 효과
	// 외곽선
	// 페이퍼 번 o

	// 맵 충돌체 이동 시스템 o
	// 카메라 페이드인 페이드아웃 o
	delete m_pFSM;
}

void CPlayerScript::awake()
{
	vPlayerFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	vPlayerUp = Transform()->GetLocalDir(DIR_TYPE::UP);

	if (nullptr != m_pFSM)
		return;

	m_pFSM = new CFSM;			  //FSM 객체 생성 
	m_pFSM->SetObject(GetObj());  //FSM의 타겟Object를  알려줌 
	
																
	TPlayerIdle* pIdleState = new TPlayerIdle;				     //여기에서 스테이트 생성 , FSM에 추가시킴 
	m_pFSM->AddState(L"Idle", pIdleState);					     // FSM에 상태를 추가한다 
	m_mapState.insert(make_pair(PLAYER_STATE::IDLE, L"Idle"));   //내 상태와 , 상태이름을 맵으로 저장한다 
	
	TPlayerRun* pRunState = new TPlayerRun;
	m_pFSM->AddState(L"Run", pRunState);
	m_mapState.insert(make_pair(PLAYER_STATE::RUN, L"Run"));
	
	TPlayerRoll* pRollState = new TPlayerRoll;
	m_pFSM->AddState(L"Roll", pRollState);
	m_mapState.insert(make_pair(PLAYER_STATE::ROLL, L"Roll"));
	
	TPlayerRollSlash* pRollSlashState = new TPlayerRollSlash;
	m_pFSM->AddState(L"Roll_Slash", pRollSlashState);
	m_mapState.insert(make_pair(PLAYER_STATE::ROLL_SLASH, L"Roll_Slash"));
	
	TPlayerSlash_Attack_L* pSlashLState = new TPlayerSlash_Attack_L;
	m_pFSM->AddState(L"Slash_L", pSlashLState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_ATTACK_L, L"Slash_L"));
	
	TPlayerSlash_Attack_R* pSlashRState = new TPlayerSlash_Attack_R;
	m_pFSM->AddState(L"Slash_R", pSlashRState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_ATTACK_R, L"Slash_R"));
	
	TPlayerCharge_L* pChargrLState = new TPlayerCharge_L;
	m_pFSM->AddState(L"Charge_L", pChargrLState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_L, L"Charge_L"));
	
	TPlayerCharge_R* pChargrRState = new TPlayerCharge_R;
	m_pFSM->AddState(L"Charge_R", pChargrRState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_R, L"Charge_R"));
	
	TPlayerChargeMax_L* pChargrMaxLState = new TPlayerChargeMax_L;
	m_pFSM->AddState(L"Charge_Max_L", pChargrMaxLState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_MAX_L, L"ChargeMax_L"));
	
	TPlayerChargeMax_R* pChargrMaxRState = new TPlayerChargeMax_R;
	m_pFSM->AddState(L"Charge_Max_R", pChargrMaxRState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_MAX_R, L"ChargeMax_R"));
	
	TPlayerCharge_Attack_L* pChargrAttackLState = new TPlayerCharge_Attack_L;
	m_pFSM->AddState(L"Charge_Attack_L", pChargrAttackLState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_ATTACK_L, L"ChargeAttack_L"));
	
	TPlayerCharge_Attack_R* pChargrAttackRState = new TPlayerCharge_Attack_R;
	m_pFSM->AddState(L"Charge_Attack_R", pChargrAttackRState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_ATTACK_R, L"ChargeAttack_R"));
	
	TPlayerArrow* pArrowState = new TPlayerArrow;
	m_pFSM->AddState(L"Arrow", pArrowState);
	m_mapState.insert(make_pair(PLAYER_STATE::ARROW, L"Arrow"));
	
	TPlayerMagic* MagicState = new TPlayerMagic;
	m_pFSM->AddState(L"Magic", MagicState);
	m_mapState.insert(make_pair(PLAYER_STATE::MAGIC, L"Magic"));
	
	TPlayerBomb* BombState = new TPlayerBomb;
	m_pFSM->AddState(L"Bomb", BombState);
	m_mapState.insert(make_pair(PLAYER_STATE::BOMB, L"Bomb"));
	
	TPlayerBomb_Finish* BombFinishState = new TPlayerBomb_Finish;
	m_pFSM->AddState(L"Bomb_Finish", BombFinishState);
	m_mapState.insert(make_pair(PLAYER_STATE::BOMB_END, L"Bomb_Finish"));
	
	TPlayerHook* HookState = new TPlayerHook;
	m_pFSM->AddState(L"Hook", HookState);
	m_mapState.insert(make_pair(PLAYER_STATE::HOOK_SHOT, L"Hook"));
	
	TPlayerHookFly* HookFlyState = new TPlayerHookFly;
	m_pFSM->AddState(L"HookFly", HookFlyState);
	m_mapState.insert(make_pair(PLAYER_STATE::HOOK_SHOT_FLY, L"HookFly"));
	
	TPlayerGet_Item* GetItemState = new TPlayerGet_Item;
	m_pFSM->AddState(L"GetItem", GetItemState);
	m_mapState.insert(make_pair(PLAYER_STATE::GET_ITEM, L"GetItem"));
	
	TPlayerHit_Back* HitBackState = new TPlayerHit_Back;
	m_pFSM->AddState(L"Hit_back", HitBackState);
	m_mapState.insert(make_pair(PLAYER_STATE::HIT_BACK, L"Hit_back"));
	
	TPlayerHit_Recover* HitRecoverState = new TPlayerHit_Recover;
	m_pFSM->AddState(L"Hit_Recover", HitRecoverState);
	m_mapState.insert(make_pair(PLAYER_STATE::HIT_RECOVER, L"Hit_Recover"));
	
	TPlayerDance* DanceState = new TPlayerDance;
	m_pFSM->AddState(L"Dance", DanceState);
	m_mapState.insert(make_pair(PLAYER_STATE::DANCE, L"Dance"));
	
	TPlayerDrown* DrownState = new TPlayerDrown;
	m_pFSM->AddState(L"Drown", DrownState);
	m_mapState.insert(make_pair(PLAYER_STATE::DROWN, L"Drown"));
	
	TPlayerLadder_Down* LadderDownState = new TPlayerLadder_Down;
	m_pFSM->AddState(L"Ladder_Down", LadderDownState);
	m_mapState.insert(make_pair(PLAYER_STATE::LADDER_DOWN, L"Ladder_Down"));
	
	TPlayerLadder_Top* LadderTopState = new TPlayerLadder_Top;
	m_pFSM->AddState(L"Ladder_Finish", LadderTopState);
	m_mapState.insert(make_pair(PLAYER_STATE::LADDER_TOP, L"Ladder_Finish"));
	
	TPlayerLadder_Up* LadderUpState = new TPlayerLadder_Up;
	m_pFSM->AddState(L"Ladder_Up", LadderUpState);
	m_mapState.insert(make_pair(PLAYER_STATE::LADDER_UP, L"Ladder_Up"));
	
	ChangeState(PLAYER_STATE::IDLE, 0.04f, L"Idle",false);
}

void CPlayerScript::start()
{
}

void CPlayerScript::update()
{
	CActorScript::update();

	PlayerPrePos = PlayerPos;

	XMMATRIX Mat = XMMatrixMultiply(g_transform.matView, g_transform.matProj);

	Vec4 Proj = DirectX::XMVector4Transform(Vec4(Transform()->GetWorldPos(), 1.f), Mat);
	m_Distance = Proj.z / Proj.w; // w 로 나눠서 실제 투영좌표 z 값을 구한다.(올바르게 비교하기 위해서)

	g_global.fDOFDistance = m_Distance;

	vPlayerFront = Transform()->GetLocalDir(DIR_TYPE::UP);
	vPlayerUp = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	PlayerPos = Transform()->GetLocalPos();
	vPlayerRot = Transform()->GetLocalRot();

	if (KEY_TAP(KEY_TYPE::NUM_1))
	{
		m_PlayerProjectileType = PLAYER_PROJECTILE_TYPE::ARROW;
	}
	else if (KEY_TAP(KEY_TYPE::NUM_2))
	{
		m_PlayerProjectileType = PLAYER_PROJECTILE_TYPE::MAGIC;
	}
	else if (KEY_TAP(KEY_TYPE::NUM_3))
	{
		m_PlayerProjectileType = PLAYER_PROJECTILE_TYPE::BOMB;
	}
	else if (KEY_TAP(KEY_TYPE::NUM_4))
	{
		m_PlayerProjectileType = PLAYER_PROJECTILE_TYPE::HOOK;
	}
	
	m_pFSM->update();
	
	if (KEY_TAP(KEY_TYPE::KEY_E))
	{
		ChangeState(PLAYER_STATE::HIT_BACK, 0.03f, L"Hit_Back", false);
	}

	PlayerMovePos = PlayerPos - PlayerPrePos;
}

void CPlayerScript::lateupdate()
{
	CActorScript::lateupdate();
	m_pFSM->lateupdate();
}

void CPlayerScript::ChangeState(PLAYER_STATE _eState, float _BlendingTime, const wstring& _AniName, bool _Stay)
{
	//FSM과 Script는 동일한 State를 가진다 
	m_eState = _eState;

	//STATE 타입을 Key로 상태 이름을 가져온다 
	wstring StateName = {}; //m_mapState.at(_eState);

	//STATE 따라 이름 바꿔줘야 함 
	map<PLAYER_STATE, wstring>::iterator iter = m_mapState.find(_eState);

	if (m_mapState.end() != iter)
	{
		StateName = iter->second;
	}
	else 
	{
		assert(0);
	}


	//FSM 에 상태변경 명령을 내린다 
	m_pFSM->ChangeState(StateName, _BlendingTime, _AniName, _Stay);
}


void CPlayerScript::KeyInput()
{
	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		ChangeState(PLAYER_STATE::RUN, 0.2f, L"Run", false);
	}

	if (KEY_TAP(KEY_TYPE::RBTN))
	{
		ChangeState(PLAYER_STATE::IDLE, 0.2f, L"Idle", false);
	}
}

Vec3 CPlayerScript::GetMouseClickPos()
{
	CGameObject* pCamera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByName(L"Camera Object");
	Vec3 CameraOrigin = pCamera->Camera()->GetRay().vPoint;
	Vec3 CameraRayDir = pCamera->Camera()->GetRay().vDir;

	Vec3 LDirCamera = -pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 RDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vec3 FDirCamera = pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 BDirCamera = -pCamera->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	FDirCamera.y = 0.f;
	BDirCamera.y = 0.f;

	FDirCamera.Normalize();
	BDirCamera.Normalize();

	Vec3 PlanePoint[4] = {};
	static float Dist = 0.f;
	/*
	0  1	
	3  2
	*/


	PlanePoint[0] = { PlayerPos + LDirCamera * 800.f + FDirCamera * 800.f };
	PlanePoint[1] = { PlayerPos + RDirCamera * 800.f + FDirCamera * 800.f };
	PlanePoint[2] = { PlayerPos + RDirCamera * 800.f + BDirCamera * 800.f };
	PlanePoint[3] = { PlayerPos + LDirCamera * 800.f + BDirCamera * 800.f };

	Vec3 IntersectPos = {};

	if (DirectX::TriangleTests::Intersects(CameraOrigin, CameraRayDir, PlanePoint[0], PlanePoint[1], PlanePoint[2], Dist))
	{			
	}
	else if (DirectX::TriangleTests::Intersects(CameraOrigin, CameraRayDir, PlanePoint[0], PlanePoint[2], PlanePoint[3], Dist))
	{		
	}

	// 평면의 방정식
	Vec3 vFlow1 = PlanePoint[1] - PlanePoint[0];
	Vec3  vFlow2 = PlanePoint[2] - PlanePoint[0];

	// 평면의 법선벡터
	Vec3  vNormal = vFlow1.Cross(vFlow2).Normalize();
	float fD = -PlanePoint[0].Dot(vNormal);

	// 직선의 방정식
	float fT = (-(vNormal.Dot(CameraOrigin)) - fD) /(CameraRayDir.Dot(vNormal));

	// 교점
	Vec3 Pos =Vec3(CameraRayDir.x * fT + CameraOrigin.x, CameraRayDir.y * fT + CameraOrigin.y, CameraRayDir.z * fT + CameraOrigin.z);

	return Pos;
}

void CPlayerScript::RotatetoClick(Vec3 _ClickPos)
{
	Vec3 relativePos = _ClickPos - PlayerPos;
	Vec3 vCross = relativePos.Cross(vPlayerFront);

	float dot = vCross.Dot(vPlayerUp);
	relativePos.Normalize();

	float RotAngle = vPlayerFront.Dot(relativePos);
	RotAngle = acos(RotAngle);

	//찍은 위치가 내 왼쪽에 있다 
	if (dot > 0.0)
		vPlayerRot.y -= RotAngle;

	//찍은 위치가 내 오른쪽에 있다 
	else if (dot < 0.0)
		vPlayerRot.y += RotAngle;

	GetObj()->Transform()->SetLocalRot(vPlayerRot);
}

void CPlayerScript::OnCollisionEnter(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;
	if ((int)LAYER_TYPE::ITEM == Obj->GetLayerIndex())
	{
		ChangeState(PLAYER_STATE::GET_ITEM, 0.3f, L"GetItem", false);
	}

	if ((int)LAYER_TYPE::MONSTER_ATTACK_COL == Obj->GetLayerIndex())
	{
		ChangeState(PLAYER_STATE::HIT_BACK, 0.3f, L"Hit_Back", false);
	}
}

void CPlayerScript::OnCollision(CGameObject* _pOther)
{
}

void CPlayerScript::OnCollisionExit(CGameObject* _pOther)
{
}

void CPlayerScript::SaveToScene(FILE* _pFile)
{
	CActorScript::SaveToScene(_pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	CActorScript::LoadFromScene(_pFile);
}
