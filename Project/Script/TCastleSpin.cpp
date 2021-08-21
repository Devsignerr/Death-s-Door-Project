#include "pch.h"
#include "TCastleSpin.h"
#include "CCastleScript.h"
#include "CRandomMgrScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleSpin::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	Vec3 Rot = GetObj()->Transform()->GetLocalRot();

	static float RotValue = 0.0f;

	if (290 <= CurAni->GetFrameIdx())
	{
		if (XM_2PI > RotValue)
		{
			Rot.y += fDT * (XM_2PI * 1.2f);
			RotValue += fDT * (XM_2PI + 0.1f);
		}
	}

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

void TCastleSpin::Enter()
{
	m_Script = (CCastleScript*)GetScript();
}

void TCastleSpin::Exit()
{
}

TCastleSpin::TCastleSpin()
	: m_Script(nullptr)
{
}

TCastleSpin::~TCastleSpin()
{
}
