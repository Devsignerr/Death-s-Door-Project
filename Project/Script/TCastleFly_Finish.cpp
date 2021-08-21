#include "pch.h"
#include "TCastleFly_Finish.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleFly_Finish::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	//

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Laser_Ready", 0.1f, L"Laser_Ready", true);
	}
}

void TCastleFly_Finish::Enter()
{
}

void TCastleFly_Finish::Exit()
{
}

TCastleFly_Finish::TCastleFly_Finish()
{
}

TCastleFly_Finish::~TCastleFly_Finish()
{
}
