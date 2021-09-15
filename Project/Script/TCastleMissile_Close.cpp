#include "pch.h"
#include "TCastleMissile_Close.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleMissile_Close::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		if (false == m_Script->RangeSearch(1500.0f))
		{
			GetFSM()->ChangeState(L"Walk", 0.1f, L"Walk", false);
		}
		else
		{
			m_Script->CheckAttackDirection();
		}
	}
}

void TCastleMissile_Close::Enter()
{
	m_Script = (CCastleScript*)GetScript();

	((CCastleScript*)GetScript())->PlaySound(L"RedeemerRevertPose", true, 0.2f);
}

void TCastleMissile_Close::Exit()
{
}

TCastleMissile_Close::TCastleMissile_Close()
	: m_Script(nullptr)
{
}

TCastleMissile_Close::~TCastleMissile_Close()
{
}
