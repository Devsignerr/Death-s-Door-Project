#include "pch.h"
#include "TCastleDeath.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleDeath::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (77 <= CurAni->GetFrameIdx())
	{
		// È¿°ú
	}
}

void TCastleDeath::Enter()
{
}

void TCastleDeath::Exit()
{
}

TCastleDeath::TCastleDeath()
{
}

TCastleDeath::~TCastleDeath()
{
}
