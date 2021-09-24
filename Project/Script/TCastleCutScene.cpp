#include "pch.h"
#include "TCastleCutScene.h"
#include "CCastleScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

void TCastleCutScene::update()
{

	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{

		GetFSM()->ChangeState(L"RightFront_HalfSpin", 0.1f, L"RightFront_HalfSpin", false);
	}
}

void TCastleCutScene::Enter()
{
	((CCastleScript*)GetScript())->PlaySound(L"RedeemerIntroPose", true, 0.5f);
}

void TCastleCutScene::Exit()
{
	CGameObject* Camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);
	CCameraScript* CameraScript = (CCameraScript*)Camera->GetScript();
	CameraScript->ResetOriginCamera();
}


TCastleCutScene::TCastleCutScene()
{
}

TCastleCutScene::~TCastleCutScene()
{
}
