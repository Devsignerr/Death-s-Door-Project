#include "pch.h"
#include "TIronCutScene.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TIronCutScene::update()
{
	
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

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
