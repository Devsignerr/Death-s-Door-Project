#include "pch.h"
#include "TCastleCutScene.h"
#include "CCastleScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TCastleCutScene::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{	
		((CCastleScript*)GetScript())->PlaySound(L"RedeemerIntroPose", true, 0.5f);
		GetFSM()->ChangeState(L"RightFront_HalfSpin", 0.1f, L"RightFront_HalfSpin", false);
	}
}

void TCastleCutScene::Enter()
{
}

void TCastleCutScene::Exit()
{
}

TCastleCutScene::TCastleCutScene()
{
}

TCastleCutScene::~TCastleCutScene()
{
}
