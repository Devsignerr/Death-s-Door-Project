#include "pch.h"
#include "TPlayerBomb_Finish.h"
#include "CPlayerScript.h"
#include "CPlayerBomb.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TPlayerBomb_Finish::Attack()
{
	if (m_BulletLimit == false)
	{
		((CPlayerBomb*)CPlayerScript::m_pBomb->GetScript())->SetBulletDir(((CPlayerScript*)GetScript())->GetPlayerFront());
		((CPlayerBomb*)CPlayerScript::m_pBomb->GetScript())->SetActive(true);

		Vec3 PlayerPos = GetObj()->Transform()->GetLocalPos();
		PlayerPos.y += 200.f;


		CPlayerScript::m_pBomb->Transform()->SetLocalPos(PlayerPos);

		CPlayerScript::m_pBomb->DisconnectWithParent();
		CPlayerScript::m_pBomb->RegisterAsParentObj();
		CPlayerScript::m_pBomb = nullptr;

		m_BulletLimit = true;
	}
}


void TPlayerBomb_Finish::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		Attack();
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerBomb_Finish::Enter()
{
}

void TPlayerBomb_Finish::Exit()
{
	CPlayerScript::m_Weapon->MeshRender()->Activate(true);
	m_BulletLimit = false;
}

TPlayerBomb_Finish::TPlayerBomb_Finish()
	: m_BulletLimit(false)
{
}

TPlayerBomb_Finish::~TPlayerBomb_Finish()
{
}
