#include "pch.h"
#include "TCastleFly_Start.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>



void TCastleFly_Start::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Fly_Stay", 0.1f, L"Fly_Stay", false);
	}
}


void TCastleFly_Start::Enter()
{
}

void TCastleFly_Start::Exit()
{
}

TCastleFly_Start::TCastleFly_Start()
{
}

TCastleFly_Start::~TCastleFly_Start()
{
}
