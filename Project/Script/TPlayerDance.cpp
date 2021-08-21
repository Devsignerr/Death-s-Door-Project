#include "pch.h"
#include "TPlayerDance.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerDance::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		//GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerDance::Enter()
{
}

void TPlayerDance::Exit()
{
}

TPlayerDance::TPlayerDance()
{
}

TPlayerDance::~TPlayerDance()
{
}
