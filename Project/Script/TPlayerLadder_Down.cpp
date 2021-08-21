#include "pch.h"
#include "TPlayerLadder_Down.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerLadder_Down::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		//GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerLadder_Down::Enter()
{
}

void TPlayerLadder_Down::Exit()
{
}

TPlayerLadder_Down::TPlayerLadder_Down()
{
}

TPlayerLadder_Down::~TPlayerLadder_Down()
{
}
