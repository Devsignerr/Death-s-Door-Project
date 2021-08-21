#include "pch.h"
#include "TPlayerArrow.h"
#include "CPlayerScript.h"
#include "CPlayerArrow.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TPlayerArrow::Attack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CPlayerArrow);

		Obj->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CPlayerArrow* Script = (CPlayerArrow*)Obj->GetScript();
		Script->SetBulletDir(GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP));

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 9);
		Obj->awake();
	}
}

void TPlayerArrow::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_HOLD(KEY_TYPE::RBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_NONE(KEY_TYPE::RBTN))
	{
		Attack();
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerArrow::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerArrow::Exit()
{
	m_BulletLimit = false;
}

TPlayerArrow::TPlayerArrow()
	: m_Script(nullptr)
	, m_BulletLimit(false)
{
}

TPlayerArrow::~TPlayerArrow()
{
}
