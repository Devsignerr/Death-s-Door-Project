#include "pch.h"
#include "TPlayerArrow.h"
#include "CPlayerScript.h"
#include "CPlayerArrow.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TPlayerArrow::Attack()
{
	if (nullptr == CPlayerScript::m_pArrow)
		return;

	if (m_BulletLimit == false)
	{
		((CPlayerArrow*)CPlayerScript::m_pArrow->GetScript())->SetBulletDir(((CPlayerScript*)GetScript())->GetPlayerFront());
		((CPlayerArrow*)CPlayerScript::m_pArrow->GetScript())->SetActive(true);
		Vec3 PlayerPos = GetObj()->Transform()->GetLocalPos();
		PlayerPos += CPlayerScript::GetPlayerUp() * 10.f;
		PlayerPos.y += 120.f;

		CPlayerScript::m_pArrow->Transform()->SetLocalPos(PlayerPos);

		Vec3 Rot = CPlayerScript::GetPlayerRot();
		CPlayerScript::m_pArrow->Transform()->SetLocalRot(Rot);

		CPlayerScript::m_pArrow->DisconnectWithParent();
		CPlayerScript::m_pArrow->RegisterAsParentObj();
		CPlayerScript::m_pArrow = nullptr;
		
		m_BulletLimit = true;

		GetScript()->Play_Sound(L"ArrowFire1", 1, true, 0.2f);
	}
}

void TPlayerArrow::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_HOLD(KEY_TYPE::RBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
	}

	if (KEY_NONE(KEY_TYPE::RBTN))
	{
		Attack();
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerArrow::Enter()
{
	//칼
	CPlayerScript::m_Weapon->MeshRender()->Activate(false);
	m_Script = (CPlayerScript*)GetScript();

	//활
	CPlayerScript::m_pBow->MeshRender()->Activate(true);

	GetScript()->Play_Sound(L"ArrowCharge3", 1, true, 0.1f);
}

void TPlayerArrow::Exit()
{
	m_BulletLimit = false;
	//칼
	CPlayerScript::m_Weapon->MeshRender()->Activate(true);
	
	//활
	CPlayerScript::m_pBow->MeshRender()->Activate(false);
}

TPlayerArrow::TPlayerArrow()
	: m_Script(nullptr)
	, m_BulletLimit(false)
{
}

TPlayerArrow::~TPlayerArrow()
{
}
