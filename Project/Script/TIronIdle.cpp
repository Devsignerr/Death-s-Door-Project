#include "pch.h"
#include "TIronIdle.h"
#include "CIronmaceScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

#include  <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

void TIronIdle::update()
{

	if (m_Script->RangeSearch(2000.0f))
		GetFSM()->ChangeState(L"CutScene", 0.03f, L"CutScene", false);
}

void TIronIdle::Enter()
{
	if (nullptr == m_Script)
		m_Script = (CIronmaceScript*)GetScript();
}

void TIronIdle::Exit()
{
	CGameObject* Camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);
	CCameraScript* CameraScript = (CCameraScript*)Camera->GetScript();
	CameraScript->CutSceneCamera(Vec3(-17502.223f, 1490.272f, 12320.024f), Vec3(2.859f / 180.f * XM_PI, 0.139f / 180.f * XM_PI, 0.0f));
}

TIronIdle::TIronIdle()
{
}

TIronIdle::~TIronIdle()
{
}