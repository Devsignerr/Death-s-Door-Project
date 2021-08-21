#include "pch.h"
#include "TCastleLeftFront_HalfSpin.h"
#include "CCastleScript.h"
#include "CRandomMgrScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleLeftFront_HalfSpin::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();
	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	static float RotValue = 0.0f;

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

void TCastleLeftFront_HalfSpin::Enter()
{
	m_Script = (CCastleScript*)GetScript();
}

void TCastleLeftFront_HalfSpin::Exit()
{
}

TCastleLeftFront_HalfSpin::TCastleLeftFront_HalfSpin()
	: m_Script(nullptr)
{
}

TCastleLeftFront_HalfSpin::~TCastleLeftFront_HalfSpin()
{
}
