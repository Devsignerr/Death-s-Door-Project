#include "pch.h"
#include "TCastleRightBack_HalfSpin.h"
#include "CCastleScript.h"
#include "CRandomMgrScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleRightBack_HalfSpin::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	static float RotValue = 0.0f;

	if (560 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(true);
		m_Script->TransColPos(Vec3(-60000.0f, -10000.0f, -25000.0f));
	}
	if (574 == CurAni->GetFrameIdx())
	{
		m_Script->OnOffAttackCol(false);
	}

	if (560 <= CurAni->GetFrameIdx())
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
			GetFSM()->ChangeState(L"Walk", 0.1f, L"Walk", false);
		}
		else
		{
			int Pattern = CRandomMgrScript::GetRandomintNumber(0, 3);

			if (3 == Pattern)
				m_Script->PatternChoice();
			else
				m_Script->CheckAttackDirection();
		}
	}
}

void TCastleRightBack_HalfSpin::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CCastleScript*)GetScript();
}

void TCastleRightBack_HalfSpin::Exit()
{
}

TCastleRightBack_HalfSpin::TCastleRightBack_HalfSpin()
	: m_Script(nullptr)
{
}

TCastleRightBack_HalfSpin::~TCastleRightBack_HalfSpin()
{
}
