#include "pch.h"
#include "TPlayerBomb_Finish.h"
#include "CPlayerScript.h"
#include "CPlayerBomb.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TPlayerBomb_Finish::Attack()
{
	if (m_BulletLimit == false)
	{
		m_BulletLimit = true;
		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CMeshRender);
		Obj->AddComponent(new CPlayerBomb);

		Obj->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
		Obj->Transform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		Obj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh_C3D"));
		Obj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl"), 0);
		CPlayerBomb* Script = (CPlayerBomb*)Obj->GetScript();
		Script->SetBulletDir(GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP));

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, 9);
		Obj->awake();
	}
}


void TPlayerBomb_Finish::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		Attack();
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerBomb_Finish::Enter()
{
}

void TPlayerBomb_Finish::Exit()
{
	m_BulletLimit = false;
}

TPlayerBomb_Finish::TPlayerBomb_Finish()
	: m_BulletLimit(false)
{
}

TPlayerBomb_Finish::~TPlayerBomb_Finish()
{
}
