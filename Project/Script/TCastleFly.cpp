#include "pch.h"
#include "TCastleFly.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleFly::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (149 <= CurAni->GetFrameIdx() && 169 >= CurAni->GetFrameIdx())
	{
		m_Script->RotateSysTem(3.0f);
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Missile_Open", 0.1f, L"Missile_Open", true);
	}
}

void TCastleFly::Enter()
{
	m_Script = (CCastleScript*)GetScript();
}

void TCastleFly::Exit()
{
}

TCastleFly::TCastleFly()
	: m_Script(nullptr)
{
}

TCastleFly::~TCastleFly()
{
}
