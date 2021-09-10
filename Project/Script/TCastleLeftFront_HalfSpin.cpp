#include "pch.h"
#include "TCastleLeftFront_HalfSpin.h"
#include "CCastleScript.h"
#include "CRandomMgrScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleLeftFront_HalfSpin::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	static float RotValue = 0.0f;

	if (670 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);

		if (!m_bImpacted)
		{
			CCameraScript::SetCameraShake(0.1f, 100.f, 6.f);
			m_Script->ActivateAttackCloud();

			m_bImpacted = true;
		}
	}

	if (670 < CurAni->GetFrameIdx() && CurAni->GetFrameIdx() < 684)
	{
		((CCastleScript*)GetScript())->ActivateImpact();
	}


	if (684 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(false);
	}


	if (668 <= CurAni->GetFrameIdx())
	{
		if (XM_PI > RotValue)
		{
			Rot.y += fDT * (XM_PI + 1.0f);
			RotValue += fDT * (XM_PI + 1.0f);
		}

	}

	//m_Script->RotateSysTem(0.5f);
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

void TCastleLeftFront_HalfSpin::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();

	m_bImpacted = false;
}

void TCastleLeftFront_HalfSpin::Exit()
{
	((CCastleScript*)GetScript())->SetImpactPTCTime(0.f);
}

TCastleLeftFront_HalfSpin::TCastleLeftFront_HalfSpin()
	: m_Script(nullptr)
{
}

TCastleLeftFront_HalfSpin::~TCastleLeftFront_HalfSpin()
{
}
