#include "pch.h"
#include "TPlayerIdle.h"
#include "TPlayerCharge_Attack_R.h"
#include "CPlayerScript.h"

#include "CSlashEffect.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CFSM.h>


void TPlayerIdle::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		if (nullptr == CPlayerScript::m_pHorizonSlashL)
		{
			CPlayerScript::m_pHorizonSlashL = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"SLASH_L", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			CPlayerScript::m_pHorizonSlashL->Transform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
			CPlayerScript::m_pHorizonSlashL->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
			CPlayerScript::m_pHorizonSlashL->Transform()->SetLocalPos(Vec3(0.f, 45.f, -170.f));
			GetObj()->AddChild(CPlayerScript::m_pHorizonSlashL);
		}
		
		((CSlashEffect*)CPlayerScript::m_pHorizonSlashL->GetScript())->SetActive(true);
		CPlayerScript::m_pHorizonSlashL->SetAllMeshrenderActive(true);

		if (nullptr != CPlayerScript::m_pHorizonSlashR)
		{
			((CSlashEffect*)CPlayerScript::m_pHorizonSlashR->GetScript())->SetActive(false);
			CPlayerScript::m_pHorizonSlashR->SetAllMeshrenderActive(false);
		}
			

		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
		((CPlayerScript*)GetScript())->ChangeState(PLAYER_STATE::SLASH_ATTACK_L, 0.1f, L"Slash_L", false);
	}

	if (KEY_HOLD(KEY_TYPE::MBTN))
	{
		TPlayerCharge_Attack_R* AttackCheck = (TPlayerCharge_Attack_R*)GetFSM()->FindState(L"Charge_Attack_R");

		if (AttackCheck->GetIsChargeAttack())
		{
			GetFSM()->ChangeState(L"Charge_R", 0.03f, L"Charge_R", false);
		}
		else
		{
			GetFSM()->ChangeState(L"Charge_L", 0.03f, L"Charge_L", false);
		}
	}

	if (KEY_HOLD(KEY_TYPE::RBTN))
	{
		PLAYER_PROJECTILE_TYPE Type = m_Script->GetPlayerProjectileType();
		switch (Type)
		{
		case PLAYER_PROJECTILE_TYPE::ARROW:
			GetFSM()->ChangeState(L"Arrow", 0.03f, L"Arrow", true);

			if (nullptr == CPlayerScript::m_pArrow)
			{
				CPlayerScript::m_pArrow = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"PlayerArrow", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			}
			
			CPlayerScript::m_pArrow->Transform()->SetLocalScale(Vec3(0.8f, 0.8f, 0.8f));
			CPlayerScript::m_pArrow->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
			CPlayerScript::m_pArrow->Transform()->SetLocalPos(Vec3(-10.f, 150.f, -10.f));
			CPlayerScript::m_pArrow->SetAllMeshrenderActive(true);
			CPlayerScript::m_pArrow->SetAllColliderActive(true);
			GetObj()->AddChild(CPlayerScript::m_pArrow);
			
			break;
		case PLAYER_PROJECTILE_TYPE::MAGIC:
		{
			GetFSM()->ChangeState(L"Magic", 0.03f, L"Magic", true);

			if (nullptr == CPlayerScript::m_pMagic)
			{
				CPlayerScript::m_pMagic = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"PlayerFire", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			}
			 CPlayerScript::m_pMagic->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			 CPlayerScript::m_pMagic->Transform()->SetLocalPos(Vec3(-60.f, 80.f, 40.f));
			 CPlayerScript::m_pMagic->ParticleSystem()->Activate(true);
			 CPlayerScript::m_pMagic->SetAllColliderActive(true);

			GetObj()->AddChild(CPlayerScript::m_pMagic);
		}
		
			break;
		case PLAYER_PROJECTILE_TYPE::BOMB:
			GetFSM()->ChangeState(L"Bomb", 0.03f, L"Bomb", true);

			if (nullptr == CPlayerScript::m_pBomb)
			{
				CPlayerScript::m_pBomb = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"PlayerBomb", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			}
			CPlayerScript::m_pBomb->Transform()->SetLocalPos(Vec3(0.f, 170.f, -100.f));
			CPlayerScript::m_pBomb->ParticleSystem()->Activate(true);
			CPlayerScript::m_pBomb->SetAllColliderActive(true);

			GetObj()->AddChild(CPlayerScript::m_pBomb);

			break;
		case PLAYER_PROJECTILE_TYPE::HOOK:
			GetFSM()->ChangeState(L"Hook", 0.03f, L"Hook", true);

			if (nullptr == CPlayerScript::m_pHook)
			{
				CPlayerScript::m_pHook = ((CPlayerScript*)GetScript())->IntanciatePrefab(L"PlayerHook", (UINT)LAYER_TYPE::PLAYER_EFFECT);
			}
			CPlayerScript::m_pHook->Transform()->SetLocalPos(Vec3(0.f, 150.f, -40.f));

			GetObj()->AddChild(CPlayerScript::m_pHook);

			break;
		default:
			break;
		}
	}


	Vec3 PlayerFront = GetObj()->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vec3 Pos = GetObj()->Transform()->GetLocalPos();
	Vec3 vPlayerPos = GetObj()->Transform()->GetLocalPos();
	Vec3 vMovePos = {  };

	vMovePos += CPlayerScript::GetOtherPower();
	bool IsGround = ((CPlayerScript*)GetScript())->GroundCheck(vPlayerPos + vMovePos);
	if (!IsGround)
		IsGround = ((CPlayerScript*)GetScript())->ResearchNode(vPlayerPos + vMovePos);
	if (true == IsGround)
		GetObj()->Transform()->SetLocalPos(vPlayerPos + vMovePos);



	if (KEY_TAP(KEY_TYPE::SPACE))
	{
		GetFSM()->ChangeState(L"Roll", 0.1f, L"Roll", false);
	}
}

void TPlayerIdle::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerIdle::Exit()
{
}

TPlayerIdle::TPlayerIdle()
	: m_Script(nullptr)
{
}

TPlayerIdle::~TPlayerIdle()
{
}
