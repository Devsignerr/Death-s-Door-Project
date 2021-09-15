#include "pch.h"
#include "TPlayerSlash_Attack_R.h"
#include "CPlayerScript.h"
#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider3D.h>


void TPlayerSlash_Attack_R::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (1140 == CurAni->GetFrameIdx())
	{
		++m_ColOnOffCheck;
	}

	if (1 == m_ColOnOffCheck)
	{
		CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol", (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
		Obj->MeshRender()->Activate(false);
		Obj->Collider3D()->Activate(false);
	}

	if (false == m_IsLeftSlash && KEY_TAP(KEY_TYPE::LBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		m_IsLeftSlash = true;
	}


	if (1150 > CurAni->GetFrameIdx())
	{
		Vec3 PlayerFront = ((CPlayerScript*)GetScript())->GetPlayerFront();
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();

		Vec3 vMovePos = {  };
		vMovePos.x += PlayerFront.x * fDT * 200.0f;
		vMovePos.z += PlayerFront.z * fDT * 200.0f;


		bool IsGround = ((CPlayerScript*)GetScript())->GroundCheck(Pos + vMovePos);

		if (!IsGround)
			IsGround = ((CPlayerScript*)GetScript())->ResearchNode(Pos + vMovePos);

		if (true == IsGround)
		{
	
			GetObj()->Transform()->SetLocalPos(Pos + vMovePos);
		}


	}

	if (1150 == CurAni->GetFrameIdx())
	{
		if (m_IsLeftSlash)
		{
			((CSlashEffect*)CPlayerScript::m_pHorizonSlashR->GetScript())->SetActive(false);
			((CSlashEffect*)CPlayerScript::m_pHorizonSlashL->GetScript())->SetActive(true);		
			CPlayerScript::m_pHorizonSlashL->SetAllMeshrenderActive(true);
			CPlayerScript::m_pHorizonSlashR->SetAllMeshrenderActive(false);

			((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::SLASH_ATTACK_L, 0.01f, L"Slash_L", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (nullptr != CPlayerScript::m_pHorizonSlashR)
			CPlayerScript::m_pHorizonSlashR->SetAllMeshrenderActive(false);

		if (nullptr != CPlayerScript::m_pHorizonSlashL)
			CPlayerScript::m_pHorizonSlashL->SetAllMeshrenderActive(false);

		((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::IDLE, 0.01f, L"Idle", false);
	}

}

void TPlayerSlash_Attack_R::Enter()
{
	GetScript()->Play_Sound(L"GreatSwordFinalSwing", 1, true, 0.2f);
	m_Script = (CPlayerScript*)GetScript();
	CGameObject* Obj = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"PlayerAttackCol", (UINT)LAYER_TYPE::PLAYER_ATTACK_COL);
	//Obj->MeshRender()->Activate(true);
	Obj->Collider3D()->Activate(true);
}

void TPlayerSlash_Attack_R::Exit()
{
	m_ColOnOffCheck = 0;
	m_IsLeftSlash = false;
}

TPlayerSlash_Attack_R::TPlayerSlash_Attack_R()
	: m_IsLeftSlash(false)
	, m_Script(nullptr)
	, m_ColOnOffCheck(0)
{

}

TPlayerSlash_Attack_R::~TPlayerSlash_Attack_R()
{
}
