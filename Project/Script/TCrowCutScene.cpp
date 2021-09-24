#include "pch.h"
#include "TCrowCutScene.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

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
	CGameObject* Camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);
	CCameraScript* CameraScript = (CCameraScript*)Camera->GetScript();
	CameraScript->CutSceneCamera(Vec3(5486.860f, 481.277f, -22277.313f), Vec3(0.450f / 180.f * XM_PI, -32.179f / 180.f * XM_PI, 0.0f));
}

void TCrowCutScene::Exit()
{
	CGameObject* Camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);
	CCameraScript* CameraScript = (CCameraScript*)Camera->GetScript();
	CameraScript->ResetOriginCamera();
}

TCrowCutScene::TCrowCutScene()
{
}

TCrowCutScene::~TCrowCutScene()
{
}
