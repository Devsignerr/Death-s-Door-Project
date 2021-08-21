#include "pch.h"
#include "TPlayerHit_Recover.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerHit_Recover::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerHit_Recover::Enter()
{
}

void TPlayerHit_Recover::Exit()
{
}

TPlayerHit_Recover::TPlayerHit_Recover()
{
}

TPlayerHit_Recover::~TPlayerHit_Recover()
{
}
