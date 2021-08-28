#include "pch.h"
#include "TIronWalk.h"
#include "CIronmaceScript.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronWalk::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	m_Script->RotateSysTem(8.0f);
	if (255 <= CurAni->GetFrameIdx() && 266 >= CurAni->GetFrameIdx() ||
		275 <= CurAni->GetFrameIdx())
	{
		Vec3 PlayerPos = CPlayerScript::GetPlayerPos();
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Vec3 Diff = PlayerPos - Pos;

		Diff.Normalize();
		Vec3 vMovePos = {};

		vMovePos.x += CTimeMgr::GetInst()->GetfDT() * Diff.x * 450.0f;
		vMovePos.z += CTimeMgr::GetInst()->GetfDT() * Diff.z * 450.0f;

		bool IsGround = m_Script->GroundCheck(Pos + vMovePos);
		if (!IsGround)
			IsGround = m_Script->ResearchNode(Pos + vMovePos);

		if (true == IsGround)
			m_Script->Transform()->SetLocalPos(Pos + vMovePos);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (m_Script->RangeSearch(1800.0f))
		{
			GetFSM()->ChangeState(L"ChopAttackCombo", 0.05f, L"ChopAttackCombo", false);
		}
		else
		{
			GetFSM()->ChangeState(L"RunAttack", 0.05f, L"RunAttack", false);
		}
	}
}

void TIronWalk::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronWalk::Exit()
{
}

TIronWalk::TIronWalk()
	: m_Script(nullptr)
{
}

TIronWalk::~TIronWalk()
{
}
