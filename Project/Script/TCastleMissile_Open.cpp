#include "pch.h"
#include "TCastleMissile_Open.h"
#include "CCastleScript.h"
#include "CCastleBullet.h"
#include "CPlayerScript.h"
#include "CRandomMgrScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TCastleMissile_Open::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (753 <= CurAni->GetFrameIdx())
	{
		m_MissileTime += fDT;

		m_MissileDelay += fDT;
		if (m_MissileDelay > 0.2f)
		{
			m_MissileDelay = 0.0f;

			Vec3 Pos = GetObj()->Transform()->GetLocalPos();
			Vec3 Right = GetObj()->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
			Vec3 Left = -GetObj()->Transform()->GetLocalDir(DIR_TYPE::RIGHT);
			MissileCreate(Pos, Right);
			MissileCreate(Pos, Left);
		}
	}

	if (m_MissileTime > 2.5f)
	{
		GetFSM()->ChangeState(L"Missile_Close", 0.1f, L"Missile_Close", false);
	}
}

void TCastleMissile_Open::Enter()
{
}

void TCastleMissile_Open::Exit()
{
	m_MissileTime = 0.0f;
	m_MissileDelay = 0.0f;
}

void TCastleMissile_Open::MissileCreate(Vec3 _Pos, Vec3 _Dir)
{
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 Up = -GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
	Vec3 Back = -GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

	CGameObject* Obj = new CGameObject;
	Obj->AddComponent(new CTransform);
	Obj->AddComponent(new CMeshRender);
	Obj->AddComponent(new CCastleBullet);

	_Pos += (_Dir * 250.0f) + (Up * 1500.0f) + (Back * 200.0f);

	Obj->Transform()->SetLocalPos(_Pos);
	Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

	Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
	Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
	CCastleBullet* Script = (CCastleBullet*)Obj->GetScript();

	int HeightTime = CRandomMgrScript::GetRandomintNumber(7, 15);
	float RealHeightTime = (float)HeightTime;
	Script->SetMaxHeight(Pos.y + 2500.0f);
	Script->SetMaxHeightTime(RealHeightTime * 0.1f);

	int TargetPos = 0;

	TargetPos = CRandomMgrScript::GetRandomintNumber(-8, 8);
	PlayerPos.x += (float)TargetPos * 100.0f;

	TargetPos = CRandomMgrScript::GetRandomintNumber(-8, 8);
	PlayerPos.z += (float)TargetPos * 100.0f;


	Script->SetTargetPos(PlayerPos);

	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CurScene->AddObject(Obj, 9);
	Obj->awake();
}

TCastleMissile_Open::TCastleMissile_Open()
	: m_MissileTime(0.0f)
	, m_MissileDelay(0.0f)
{
}

TCastleMissile_Open::~TCastleMissile_Open()
{
}
