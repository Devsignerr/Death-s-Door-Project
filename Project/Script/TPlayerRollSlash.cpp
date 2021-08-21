#include "pch.h"
#include "TPlayerRollSlash.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerRollSlash::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerRollSlash::Enter()
{
}

void TPlayerRollSlash::Exit()
{
}

TPlayerRollSlash::TPlayerRollSlash()
{
}

TPlayerRollSlash::~TPlayerRollSlash()
{
}
