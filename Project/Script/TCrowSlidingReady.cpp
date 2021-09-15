#include "pch.h"
#include "TCrowSlidingReady.h"
#include "CCrowScript.h"
#include "CCrowEggBullet.h"
#include "CPlayerScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

void TCrowSlidingReady::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Sliding", 0.03f, L"Sliding", false);
	}
}

void TCrowSlidingReady::Enter()
{
	const vector<CGameObject*>& Temp = CSceneMgr::GetInst()->GetCurScene()->GetLayer((UINT)LAYER_TYPE::INDETERMINATE)->GetObjects();

	if (true == Temp.empty())
	{
		Vec3 Pos = GetObj()->Transform()->GetLocalPos();
		Pos.y += 150.0f;

		CGameObject* Obj = new CGameObject;
		Obj->AddComponent(new CTransform);
		Obj->AddComponent(new CCrowEggBullet);
		Obj->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());

		CCrowEggBullet* Script = (CCrowEggBullet*)Obj->GetScript();
		Script->SetStartPos(Pos);
		Script->SetDir(GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP));
		Script->SetCrowBossRot(GetObj()->Transform()->GetLocalRot());

		float Range = Vec3::Distance(CPlayerScript::GetPlayerPos(), Pos) + 4000.0f;
		Script->SetRange(Range);

		Script->awake();

		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->AddObject(Obj, (UINT)LAYER_TYPE::INDETERMINATE);
	}
}

void TCrowSlidingReady::Exit()
{
	m_LastDir = GetObj()->Transform()->GetLocalDir(DIR_TYPE::UP);
}

TCrowSlidingReady::TCrowSlidingReady()
{
}

TCrowSlidingReady::~TCrowSlidingReady()
{
}
