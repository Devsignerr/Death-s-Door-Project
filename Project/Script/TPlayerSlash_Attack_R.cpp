#include "pch.h"
#include "TPlayerSlash_Attack_R.h"
#include "CPlayerScript.h"
#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerSlash_Attack_R::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


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

			GetFSM()->ChangeState(L"Slash_L", 0.01f, L"Slash_L", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.01f, L"Idle", false);
	}

}

void TPlayerSlash_Attack_R::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerSlash_Attack_R::Exit()
{
	m_IsLeftSlash = false;
}

TPlayerSlash_Attack_R::TPlayerSlash_Attack_R()
	: m_IsLeftSlash(false)
	, m_Script(nullptr)
{

}

TPlayerSlash_Attack_R::~TPlayerSlash_Attack_R()
{
}
