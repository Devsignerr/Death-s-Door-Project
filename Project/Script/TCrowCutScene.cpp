#include "pch.h"
#include "TCrowCutScene.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCrowCutScene::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
}

void TCrowCutScene::Enter()
{
}

void TCrowCutScene::Exit()
{
}

TCrowCutScene::TCrowCutScene()
{
}

TCrowCutScene::~TCrowCutScene()
{
}
