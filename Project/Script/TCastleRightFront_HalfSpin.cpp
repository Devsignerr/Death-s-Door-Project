#include "pch.h"
#include "TCastleRightFront_HalfSpin.h"
#include "CCastleScript.h"
#include "CRandomMgrScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleRightFront_HalfSpin::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	static float RotValue = 0.0f;

	if (605 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);
		//m_Script->TransColPos(Vec3(-60000.0f, -10000.0f, 25000.0f));

		if (!m_bImpacted)
		{
			CCameraScript::SetCameraShake(0.1f, 100.f, 6.f);
			m_Script->ActivateAttackCloud();

			m_bImpacted = true;
		}
	}

	if (605 < CurAni->GetFrameIdx() && CurAni->GetFrameIdx() < 619)
	{
		((CCastleScript*)GetScript())->ActivateImpact();

	}

	if (619 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(false);
	}

	// 605
	if (605 <= CurAni->GetFrameIdx())
	{
		if (XM_PI > RotValue)
		{
			Rot.y -= fDT * (XM_PI + 1.0f);
			RotValue += fDT * (XM_PI + 1.0f);
		}

	}

	GetObj()->Transform()->SetLocalRot(Rot);
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		RotValue = 0.0f;

		if (false == m_Script->RangeSearch(1500.0f))
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 1);

			if (1 == Pattern)
				m_Script->PatternChoice();
			else
				GetFSM()->ChangeState(L"Walk", 0.1f, L"Walk", false);
		}
		else
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 2);

			if (2 == Pattern)
				m_Script->PatternChoice();
			else
				m_Script->CheckAttackDirection();
		}
	}
}

void TCastleRightFront_HalfSpin::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();

	m_bImpacted = false;
}

void TCastleRightFront_HalfSpin::Exit()
{
	((CCastleScript*)GetScript())->SetImpactPTCTime(0.f);
}

TCastleRightFront_HalfSpin::TCastleRightFront_HalfSpin()
	: m_Script(nullptr)
{
}

TCastleRightFront_HalfSpin::~TCastleRightFront_HalfSpin()
{
}
