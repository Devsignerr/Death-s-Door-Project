#include "pch.h"
#include "TPlayerLadder_Top.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerLadder_Top::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		//GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerLadder_Top::Enter()
{
}

void TPlayerLadder_Top::Exit()
{
}

TPlayerLadder_Top::TPlayerLadder_Top()
{
}

TPlayerLadder_Top::~TPlayerLadder_Top()
{
}
