#include "pch.h"
#include "TCrowGuidedBullet.h"
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowGuidedBullet::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
}

void TCrowGuidedBullet::Enter()
{
}

void TCrowGuidedBullet::Exit()
{
}

TCrowGuidedBullet::TCrowGuidedBullet()
{
}

TCrowGuidedBullet::~TCrowGuidedBullet()
{
}
