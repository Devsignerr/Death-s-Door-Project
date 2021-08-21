#include "pch.h"
#include "TPlayerHookFly.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerHookFly::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		//GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerHookFly::Enter()
{
}

void TPlayerHookFly::Exit()
{
}

TPlayerHookFly::TPlayerHookFly()
{
}

TPlayerHookFly::~TPlayerHookFly()
{
}
