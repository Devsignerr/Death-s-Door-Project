#include "pch.h"
#include "TPlayerMagic.h"
#include "CPlayerScript.h"
#include "CPlayerMagic.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>
#include <Engine/CScene.h>
#include <Engine/CSceneMgr.h>

void TPlayerMagic::Attack()
{
	if (m_BulletLimit == false)
	{		
		((CPlayerMagic*)CPlayerScript::m_pMagic->GetScript())->SetBulletDir(((CPlayerScript*)GetScript())->GetPlayerFront());
		((CPlayerMagic*)CPlayerScript::m_pMagic->GetScript())->SetActive(true);

		Vec3 PlayerPos = GetObj()->Transform()->GetLocalPos();
		PlayerPos.y += 110.f;

		CPlayerScript::m_pMagic->Transform()->SetLocalPos(PlayerPos);

		CPlayerScript::m_pMagic->DisconnectWithParent();
		CPlayerScript::m_pMagic->RegisterAsParentObj();
		CPlayerScript::m_pMagic = nullptr;

		m_BulletLimit = true;

		GetScript()->Play_Sound(L"FireBallFire1", 1, true, 0.2f);
	}
}

void TPlayerMagic::update()
{
	m_fCurTime += fDT;

	if (m_fRotateTime > m_fCurTime)
	{
		float x = 20.f - sin(m_fCurTime*1.5f) * m_fRadius;
		float y = 100.f;
		float z = 40.f - cos(m_fCurTime*2.0f) * m_fRadius;

		CPlayerScript::m_pMagic->Transform()->SetLocalPos(Vec3(x, y, z));
	}

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

void TPlayerMagic::Enter()
{
	m_Script = (CPlayerScript*)GetScript();

	CPlayerScript::m_Weapon->MeshRender()->Activate(false);

	GetScript()->Play_Sound(L"FireBallCharge1", 1, true, 0.2f);
}

void TPlayerMagic::Exit()
{
	m_BulletLimit = false;
	m_fCurTime = 0.f;

	CPlayerScript::m_Weapon->MeshRender()->Activate(true);
}

TPlayerMagic::TPlayerMagic()
	: m_Script(nullptr)
	, m_BulletLimit(false)
	, m_fRotateTime(0.8f)
	, m_fRadius(90.f)
{
}

TPlayerMagic::~TPlayerMagic()
{
}
