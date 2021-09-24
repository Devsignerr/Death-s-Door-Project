#include "pch.h"
#include "TPlayerHit_Back.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CCollider3D.h>

void TPlayerHit_Back::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Hit_Recover", 0.03f, L"Hit_Recover", false);
	}
}

void TPlayerHit_Back::Enter()
{
	CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol", (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
	Obj->Collider3D()->Activate(false);
}

void TPlayerHit_Back::Exit()
{
}

TPlayerHit_Back::TPlayerHit_Back()
{
}

TPlayerHit_Back::~TPlayerHit_Back()
{
}
