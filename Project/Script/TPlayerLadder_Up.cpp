#include "pch.h"
#include "TPlayerLadder_Up.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerLadder_Up::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		//GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerLadder_Up::Enter()
{
}

void TPlayerLadder_Up::Exit()
{
}

TPlayerLadder_Up::TPlayerLadder_Up()
{
}

TPlayerLadder_Up::~TPlayerLadder_Up()
{
}
