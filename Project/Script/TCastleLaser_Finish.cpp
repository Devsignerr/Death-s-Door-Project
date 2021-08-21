#include "pch.h"
#include "TCastleLaser_Finish.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleLaser_Finish::update()
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

void TCastleLaser_Finish::Enter()
{
	m_Script = (CCastleScript*)GetScript();
}

void TCastleLaser_Finish::Exit()
{
}

TCastleLaser_Finish::TCastleLaser_Finish()
	: m_Script(nullptr)
{
}

TCastleLaser_Finish::~TCastleLaser_Finish()
{
}
