#include "pch.h"
#include "TPlayerRollSlash.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CCollider3D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTransform.h>

void TPlayerRollSlash::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (1637 == CurAni->GetFrameIdx())
	{
		++m_ColOnOffCheck;
	}

	if (1 == m_ColOnOffCheck)
	{
		CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol",(UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
		Obj->MeshRender()->Activate(false);
		Obj->Collider3D()->Activate(false);


		Obj->Transform()->SetLocalScale(Vec3(3.0f, 2.0f, 1.0f));
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerRollSlash::Enter()
{
	CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol", (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
	Obj->MeshRender()->Activate(true);
	Obj->Collider3D()->Activate(true);

	Obj->Transform()->SetLocalScale(Vec3(1.0f, 3.0f, 2.0f));
}

void TPlayerRollSlash::Exit()
{
	m_ColOnOffCheck = 0;
}

TPlayerRollSlash::TPlayerRollSlash()
{
}

TPlayerRollSlash::~TPlayerRollSlash()
{
}
