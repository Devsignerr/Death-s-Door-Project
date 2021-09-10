#include "pch.h"
#include "TCastleSpin.h"
#include "CCastleScript.h"
#include "CRandomMgrScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleSpin::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	if (284 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);

		if (!m_bImpacted)
		{
			CCameraScript::SetCameraShake(0.1f, 100.f, 6.f);
			m_Script->ActivateAttackCloud();

			m_bImpacted = true;
		}
	}

	if (284 < CurAni->GetFrameIdx() && CurAni->GetFrameIdx() < 320)
	{
		((CCastleScript*)GetScript())->ActivateImpact();
	}


	if (320 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(false);
	}

	static float RotValue = 0.0f;

	if (290 <= CurAni->GetFrameIdx())
	{
		if (XM_2PI > RotValue)
		{
			Rot.y += fDT * (XM_2PI * 1.0f);
			RotValue += fDT * (XM_2PI + 0.1f);
		}
	}

	GetObj()->Transform()->SetLocalRot(Rot);

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		RotValue = 0.0f;

		if (false == m_Script->RangeSearch(1500.0f))
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0,1);

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

void TCastleSpin::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();

	m_bImpacted = false;
}

void TCastleSpin::Exit()
{
	((CCastleScript*)GetScript())->SetImpactPTCTime(0.f);
}

TCastleSpin::TCastleSpin()
	: m_Script(nullptr)
{
}

TCastleSpin::~TCastleSpin()
{
}
