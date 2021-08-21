#include "pch.h"
#include "TCrowSlidingRot.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowSlidingRot::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Sliding", 0.03f, L"Sliding", false);
	}
}

void TCrowSlidingRot::Enter()
{
}

void TCrowSlidingRot::Exit()
{
}

TCrowSlidingRot::TCrowSlidingRot()
{
}

TCrowSlidingRot::~TCrowSlidingRot()
{
}
