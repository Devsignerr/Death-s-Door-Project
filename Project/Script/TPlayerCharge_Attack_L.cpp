#include "pch.h"
#include "TPlayerCharge_Attack_L.h"
#include "TPlayerCharge_Attack_R.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CFSM.h>

void TPlayerCharge_Attack_L::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (1175 == CurAni->GetFrameIdx())
	{
		++m_ColOnOffCheck;
	}

	if (1 == m_ColOnOffCheck)
	{
		CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol",(UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
		Obj->MeshRender()->Activate(false);
		Obj->Collider3D()->Activate(false);
	}

	if (1185 > CurAni->GetFrameIdx())
	{
		Vec3 PlayerFront = ((CPlayerScript*)GetScript())->GetPlayerFront();
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Vec3 vMovePos = {  };
		vMovePos.x += PlayerFront.x * fDT * 100.0f;
		vMovePos.z += PlayerFront.z * fDT * 100.0f;
		bool IsGround = ((CPlayerScript*)GetScript())->GroundCheck(Pos + vMovePos);

		if (!IsGround)
			IsGround = ((CPlayerScript*)GetScript())->ResearchNode(Pos + vMovePos);

		if (true == IsGround)
		{
	
			GetObj()->Transform()->SetLocalPos(Pos + vMovePos);
		}
	
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		if (nullptr != CPlayerScript::m_pHeavySlashR)
			CPlayerScript::m_pHeavySlashR->SetAllMeshrenderActive(false);

		if (nullptr != CPlayerScript::m_pHeavySlashL)
			CPlayerScript::m_pHeavySlashL->SetAllMeshrenderActive(false);


		((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::IDLE, 0.01f, L"Idle", false);
	}
}

void TPlayerCharge_Attack_L::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
	Vec3 Pos = m_Script->GetMouseClickPos();
	m_Script->RotatetoClick(Pos);

	TPlayerCharge_Attack_R* AttackCheck = (TPlayerCharge_Attack_R*)GetFSM()->FindState(L"Charge_Attack_R");
	AttackCheck->ResetIsChargeAttack();

	CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol", (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
	Obj->MeshRender()->Activate(true);
	Obj->Collider3D()->Activate(true);
}

void TPlayerCharge_Attack_L::Exit()
{
	m_ColOnOffCheck = 0;
}

TPlayerCharge_Attack_L::TPlayerCharge_Attack_L()
	: m_Script(nullptr)
{
}

TPlayerCharge_Attack_L::~TPlayerCharge_Attack_L()
{
}
