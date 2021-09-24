#include "pch.h"
#include "CPlayerScript.h"
#include <Engine\CSceneMgr.h>
#include <Engine\CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CCamera.h>
#include <Engine/CFSM.h>
#include <Engine/CCollider3D.h>
#include <Engine/CState.h>
#include <Engine/CParticleSystem.h>

#include <Script/CCameraScript.h>
#include <Script/CFadeScript.h>
#include "CPlayerArrow.h"
#include "CPlayerBomb.h"
#include "CPlayerMagic.h"
#include "CPlayerHook.h"
#include "CSlashEffect.h"

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

#include "CHpBar.h"

#pragma endregion

CPlayerScript* CPlayerScript::Player = {};
Vec3 CPlayerScript::PlayerMovePos = {};
Vec3 CPlayerScript::PlayerPrePos = {};
Vec3 CPlayerScript::PlayerPos = {};
Vec3 CPlayerScript::vPlayerRot = {};
Vec3 CPlayerScript::vPlayerFront = {};
Vec3 CPlayerScript::vPlayerUp = {};
Vec3 CPlayerScript::m_OtherPower = {};

CGameObject* CPlayerScript::m_pMagic = nullptr;
CGameObject* CPlayerScript::m_pArrow = nullptr;
CGameObject* CPlayerScript::m_pBomb = nullptr;
CGameObject* CPlayerScript::m_pHook = nullptr;

CGameObject* CPlayerScript::m_Weapon = nullptr;
CGameObject* CPlayerScript::m_pBow = nullptr;

CGameObject* CPlayerScript::m_pRollSlash =nullptr;
CGameObject* CPlayerScript::m_pHorizonSlashL = nullptr;
CGameObject* CPlayerScript::m_pHorizonSlashR = nullptr;

CGameObject* CPlayerScript::m_pHeavySlashL = nullptr;
CGameObject* CPlayerScript::m_pHeavySlashR = nullptr;

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
	delete m_pFSM;

	 CPlayerScript::m_pMagic = nullptr;
	 CPlayerScript::m_pArrow = nullptr;
	 CPlayerScript::m_pBomb = nullptr;
	 CPlayerScript::m_pHook = nullptr;
	
	 CPlayerScript::m_Weapon = nullptr;
	 CPlayerScript::m_pBow = nullptr;
	
	 CPlayerScript::m_pRollSlash = nullptr;
	 CPlayerScript::m_pHorizonSlashL = nullptr;
	 CPlayerScript::m_pHorizonSlashR = nullptr;
	 CPlayerScript::m_pHeavySlashL = nullptr;
	 CPlayerScript::m_pHeavySlashR = nullptr;

}

void CPlayerScript::awake()
{
	CreateCol();

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
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_MAX_L, L"Charge_Max_L"));
	
	TPlayerChargeMax_R* pChargrMaxRState = new TPlayerChargeMax_R;
	m_pFSM->AddState(L"Charge_Max_R", pChargrMaxRState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_MAX_R, L"Charge_Max_R"));
	
	TPlayerCharge_Attack_L* pChargrAttackLState = new TPlayerCharge_Attack_L;
	m_pFSM->AddState(L"Charge_Attack_L", pChargrAttackLState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_ATTACK_L, L"Charge_Attack_L"));
	
	TPlayerCharge_Attack_R* pChargrAttackRState = new TPlayerCharge_Attack_R;
	m_pFSM->AddState(L"Charge_Attack_R", pChargrAttackRState);
	m_mapState.insert(make_pair(PLAYER_STATE::SLASH_CHARGE_ATTACK_R, L"Charge_Attack_R"));
	
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

	const vector<CGameObject*>& vecChild = GetObj()->GetChild();
	//활
	m_Weapon = vecChild[1];
	m_pBow = vecChild[2];
	m_pBow->MeshRender()->Activate(false);

	PlayerPos= Transform()->GetLocalPos();
	PlayerPrePos = Transform()->GetLocalPos();

	ResetNavMeshInfo();
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

	vPlayerFront = -Transform()->GetLocalDir(DIR_TYPE::FRONT);
	vPlayerUp = Transform()->GetLocalDir(DIR_TYPE::UP);
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


void CPlayerScript::CreateCol()
{

	{
		Vec3 Pos = Transform()->GetLocalPos();
		CGameObject* Obj = new CGameObject;
		Obj->SetName(L"PlayerCol");
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCollider3D);

		Obj->Transform()->SetLocalPos(Vec3(0.0f, 100.0f, 0.0f));
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 200.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		Obj->MeshRender()->Activate(false);

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::PLAYER_COL);

		GetObj()->AddChild(Obj);

		Obj->MeshRender()->Activate(false);
	}
	{
		Vec3 Pos = Transform()->GetLocalPos();
		CGameObject* Obj = new CGameObject;
		Obj->SetName(L"PlayerAttackCol");
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CCollider3D);

		Obj->Transform()->SetLocalPos(Vec3(0.0f, 150.0f, -500.f));
		Obj->Transform()->SetLocalScale(Vec3(500.0f, 150.0f, 400.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);

		Obj->Collider3D()->SetParentOffsetPos(Vec3(0.0f, 200.0f, -250.f));

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
		GetObj()->AddChild(Obj);

		Obj->MeshRender()->Activate(false);
		Obj->Collider3D()->Activate(false);
	}
}

void CPlayerScript::ClearAllProjectile()
{
	if (m_pArrow)
	{
		m_pArrow->SetAllMeshrenderActive(false);
		m_pArrow->SetAllColliderActive(false);
		m_pArrow->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		((CPlayerArrow*)m_pArrow->GetScript())->SetActive(false);

		if (nullptr != m_pArrow->GetParent())
		{
			m_pArrow->DisconnectWithParent();
			m_pArrow->RegisterAsParentObj();
		}
			

		DeleteObject(m_pArrow);

		m_pArrow = nullptr;
	}

	if (m_pMagic)
	{
		m_pMagic->ParticleSystem()->Activate(false);
		m_pMagic->SetAllColliderActive(false);
		m_pMagic->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		((CPlayerMagic*)m_pMagic->GetScript())->SetActive(false);

		if (nullptr != m_pMagic->GetParent())
		{
			m_pMagic->DisconnectWithParent();
			m_pMagic->RegisterAsParentObj();
		}
			

		m_pMagic->ParticleSystem()->Destroy();
	
		m_pMagic = nullptr;
	}

	if (m_pBomb)
	{
		m_pBomb->ParticleSystem()->Activate(false);
		m_pBomb->SetAllColliderActive(false);
		m_pBomb->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		((CPlayerBomb*)m_pBomb->GetScript())->SetActive(false);

		if (nullptr != m_pBomb->GetParent())
		{
			m_pBomb->DisconnectWithParent();
			m_pBomb->RegisterAsParentObj();
		}	

		m_pBomb->ParticleSystem()->Destroy();

		m_pBomb = nullptr;
	}

	if (m_pHook)
	{
		if (nullptr != m_pHook->GetParent())
		{
			m_pHook->DisconnectWithParent();
			m_pHook->RegisterAsParentObj();
		}		

		((CPlayerHook*)m_pHook->GetScript())->Destroy();

		m_pHook = nullptr;
	}

	if (nullptr != m_pHeavySlashR)
	{
		((CSlashEffect*)CPlayerScript::m_pHeavySlashR->GetScript())->SetActive(false);
		m_pHeavySlashR->SetAllMeshrenderActive(false);
	}

	if (nullptr != m_pHeavySlashL)
	{
		((CSlashEffect*)CPlayerScript::m_pHeavySlashL->GetScript())->SetActive(false);
		m_pHeavySlashL->SetAllMeshrenderActive(false);
	}

	if (nullptr != m_pHorizonSlashR)
	{
		((CSlashEffect*)CPlayerScript::m_pHorizonSlashR->GetScript())->SetActive(false);
		m_pHorizonSlashR->SetAllMeshrenderActive(false);
	}

	if (nullptr != m_pHorizonSlashL)
	{
		((CSlashEffect*)CPlayerScript::m_pHorizonSlashL->GetScript())->SetActive(false);
		m_pHorizonSlashL->SetAllMeshrenderActive(false);
	}
	
	if (nullptr != m_pRollSlash)
	{
		((CSlashEffect*)CPlayerScript::m_pRollSlash->GetScript())->SetActive(false);
		m_pRollSlash->SetAllMeshrenderActive(false);
	}


}


void CPlayerScript::KeyInput()
{
	
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


	PlanePoint[0] = { PlayerPos + LDirCamera * 10000.f + FDirCamera * 10000.f };
	PlanePoint[1] = { PlayerPos + RDirCamera * 10000.f + FDirCamera * 10000.f };
	PlanePoint[2] = { PlayerPos + RDirCamera * 10000.f + BDirCamera * 10000.f };
	PlanePoint[3] = { PlayerPos + LDirCamera * 10000.f + BDirCamera * 10000.f };

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
	Vec3 vCross = relativePos.Cross(GetPlayerFront());

	float dot = vCross.Dot(GetPlayerUp());
	relativePos.Normalize();

	float RotAngle = GetPlayerFront().Dot(relativePos);

	if (RotAngle > 1.f)
		RotAngle = 1.f;
	else if (RotAngle < -1.f)
		RotAngle = -1.f;


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

	if ((int)LAYER_TYPE::MONSTER_ATTACK_COL == Obj->GetLayerIndex() ||
		(int)LAYER_TYPE::MONSTER_BULLET_COL == Obj->GetLayerIndex() ||
		(int)LAYER_TYPE::BOSS_ATTACK_COL == Obj->GetLayerIndex() ||
		(int)LAYER_TYPE::BOSS_BULLET_COL == Obj->GetLayerIndex())
	{
		if (m_eState != PLAYER_STATE::HIT_BACK && m_eState != PLAYER_STATE::HIT_RECOVER && m_eState != PLAYER_STATE::ROLL)
		{
			ClearAllProjectile();
			CHpBar::m_HitCheck = true;
			ChangeState(PLAYER_STATE::HIT_BACK, 0.3f, L"Hit_Back", false);
		}
	}


	if ((int)LAYER_TYPE::MONSTER_COL == Obj->GetLayerIndex() || (int)LAYER_TYPE::BOSS_COL == Obj->GetLayerIndex())
	{
		CCameraScript::SetCameraShake(0.2f, 100.f, 3.f);

		while (nullptr != _pOther->GetParent())
		{
			_pOther = _pOther->GetParent();
		}

		Vec3 OtherPos = _pOther->Transform()->GetLocalPos();

		Vec3 vDiff = OtherPos - GetPlayerPos();

		ActivateImpactParticle(Vec4(0.f, 0.f, 0.f, 0.f), OtherPos, -vDiff, 25, 20);

		Play_Sound(L"EnemyHit1", 1, true, 0.5f);
	}

	else if ((UINT)LAYER_TYPE::MAP_GIMIC_COL == Obj->GetLayerIndex() || (UINT)LAYER_TYPE::WALL_COL == Obj->GetLayerIndex())
	{
		Transform()->SetLocalPos(PlayerPrePos);
	}

}

void CPlayerScript::OnCollision(CGameObject* _pOther)
{
	CGameObject* Obj = _pOther;

	if ((UINT)LAYER_TYPE::MAP_GIMIC_COL == Obj->GetLayerIndex() || (UINT)LAYER_TYPE::WALL_COL == Obj->GetLayerIndex())
	{
		Transform()->SetLocalPos(PlayerPrePos);
	}
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
