#include "pch.h"
#include "TPlayerHit_Back.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerHit_Back::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Hit_Recover", 0.03f, L"Hit_Recover", false);
	}
}

void TPlayerHit_Back::Enter()
{
}

void TPlayerHit_Back::Exit()
{
}

TPlayerHit_Back::TPlayerHit_Back()
{
}

TPlayerHit_Back::~TPlayerHit_Back()
{
}
