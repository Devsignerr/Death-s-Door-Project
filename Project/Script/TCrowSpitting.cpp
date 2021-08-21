#include "pch.h"
#include "TCrowSpitting.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowSpitting::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
			GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
}

void TCrowSpitting::Enter()
{
}

void TCrowSpitting::Exit()
{
}

TCrowSpitting::TCrowSpitting()
{
}

TCrowSpitting::~TCrowSpitting()
{
}
