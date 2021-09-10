#include "pch.h"
#include "TCrowBatBullet.h"
#include "CCrowBatBullet.h"
#include "CRandomMgrScript.h"
#include "CMemoryPoolScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider3D.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>

void TCrowBatBullet::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	m_Reload += fDT;

	if (460 < CurAni->GetFrameIdx() && 513 > CurAni->GetFrameIdx() && m_Reload > 0.05f)
	{
		m_Reload = 0.0f;

		static float StartRad = 0.0f;
		static float Change = 1.0f;

		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Vec3 Rot = GetObj()->Transform()->GetLocalRot();

		Vec3 Front = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
		Vec3 Up = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

		Rot.y += StartRad;

		CGameObject* Obj = nullptr;
		Obj = CMemoryPoolScript::GetCrowBullet();

		if (Obj != nullptr)
		{
			Pos += (Front * 700.0f) + (Up * 700.0f);

			Vec3 ObjRot = Obj->Transform()->GetLocalRot();

			Obj->Transform()->SetLocalPos(Pos);
			Obj->Transform()->SetLocalRot(Rot);

			CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();

			CurScene->AddObject(Obj, (UINT)LAYER_TYPE::BOSS_EFFECT);

			Obj->awake();

			int Temp = CRandomMgrScript::GetRandomintNumber(0, 1);
			int Temp2 = CRandomMgrScript::GetRandomintNumber(6, 18);

			//if (StartRad > XM_PI / 6.0f)
			//	Change = -1.0f;
			//else if (StartRad < -XM_PI / 6.0f)
			//	Change = 1.0f;

			if (0 == Temp)
				Change = -1.0f;
			else
				Change = 1.0f;

			StartRad = (XM_PI / Temp2) * Change;
		}
	}



	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
}

void TCrowBatBullet::Enter()
{
}

void TCrowBatBullet::Exit()
{
}

TCrowBatBullet::TCrowBatBullet()
	: m_Reload(0.0f)
{
}

TCrowBatBullet::~TCrowBatBullet()
{
}
