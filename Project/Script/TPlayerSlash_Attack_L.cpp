#include "pch.h"
#include "TPlayerSlash_Attack_L.h"
#include "CPlayerScript.h"
#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerSlash_Attack_L::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (false == m_IsRightSlash && KEY_TAP(KEY_TYPE::LBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		m_IsRightSlash = true;
	}

	if (1185 > CurAni->GetFrameIdx())
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
	if (1185 == CurAni->GetFrameIdx())
	{
		if (m_IsRightSlash && m_LimitAttackCount != 2)
		{
			if (nullptr == CPlayerScript::m_pHorizonSlashR)
			{
				CPlayerScript::m_pHorizonSlashR = ((CPlayerScript*)GetScript())->IstanciatePrefab(L"SLASH_Rd", (UINT)LAYER_TYPE::PLAYER_EFFECT_DONSAVE);
				CPlayerScript::m_pHorizonSlashR->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
				CPlayerScript::m_pHorizonSlashR->Transform()->SetLocalRot(Vec3(0.f, XM_PI, 0.f));
				CPlayerScript::m_pHorizonSlashR->Transform()->SetLocalPos(Vec3(0.f, 30.f, -170.f));
				GetObj()->AddChild(CPlayerScript::m_pHorizonSlashR);
			}

			((CSlashEffect*)CPlayerScript::m_pHorizonSlashR->GetScript())->SetActive(true);
			((CSlashEffect*)CPlayerScript::m_pHorizonSlashL->GetScript())->SetActive(false);

			CPlayerScript::m_pHorizonSlashR->SetAllMeshrenderActive(true);
			CPlayerScript::m_pHorizonSlashL->SetAllMeshrenderActive(false);
			
			GetFSM()->ChangeState(L"Slash_R", 0.01f, L"Slash_R", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.01f, L"Idle", false);
	}

}

void TPlayerSlash_Attack_L::Enter()
{
	++m_LimitAttackCount;
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerSlash_Attack_L::Exit()
{
	m_IsRightSlash = false;

	if (2 == m_LimitAttackCount)
	{
		m_LimitAttackCount = 0;
	}
}

TPlayerSlash_Attack_L::TPlayerSlash_Attack_L()
	: m_IsRightSlash(false)
	, m_LimitAttackCount(0)
	, m_Script(nullptr)
{

}

TPlayerSlash_Attack_L::~TPlayerSlash_Attack_L()
{
}