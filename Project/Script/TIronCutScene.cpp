#include "pch.h"
#include "TIronCutScene.h"
#include "CIronmaceScript.h"
#include "CCameraScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CCollider3D.h>
#include <Engine/CFSM.h>
#include  <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

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
	vector<CGameObject*>& Temp = (vector<CGameObject*>&)CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::SCENE_CHANGE_COL)->GetObjects();

	vector<CGameObject*>::iterator iter = Temp.begin();

	for (; iter != Temp.end(); ++iter)
	{
		if (nullptr!=(*iter)->Collider3D())
		{
			Vec3 LocalPos = ((*iter)->Transform())->GetLocalPos();
			LocalPos.y += 10000.f;
			((*iter)->Transform())->SetLocalPos(LocalPos);
		}
	}
}

void TIronCutScene::Exit()
{
	CGameObject* Camera = CSceneMgr::GetInst()->GetCurScene()->FindObjectByLayer(L"Camera Object", (UINT)LAYER_TYPE::CAMERA);
	CCameraScript* CameraScript = (CCameraScript*)Camera->GetScript();
	CameraScript->ResetOriginCamera();
}

TIronCutScene::TIronCutScene()
{
}

TIronCutScene::~TIronCutScene()
{
}
