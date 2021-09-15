#include "pch.h"
#include "TIronCutScene.h"
#include "CIronmaceScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronCutScene::update()
{
	
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (397 == CurAni->GetFrameIdx())
	{
		((CIronmaceScript*)GetScript())->PlaySound(L"Revert4", true, 0.5f);
	}

	if (430 == CurAni->GetFrameIdx())
	{
		((CIronmaceScript*)GetScript())->PlaySound(L"KnightSlam1", true, 0.8f);
	}

	if(460== CurAni->GetFrameIdx())
	{
		((CIronmaceScript*)GetScript())->PlaySound(L"Swing1", true, 0.4f);
	}
		
	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Walk", 0.1f, L"Walk", false);
	}
}

void TIronCutScene::Enter()
{
}

void TIronCutScene::Exit()
{
}

TIronCutScene::TIronCutScene()
{
}

TIronCutScene::~TIronCutScene()
{
}
