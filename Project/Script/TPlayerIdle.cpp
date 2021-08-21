#include "pch.h"
#include "TPlayerIdle.h"
#include "TPlayerCharge_Attack_R.h"
#include "CPlayerScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerIdle::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
	else if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}
	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		GetFSM()->ChangeState(L"Run", 0.03f, L"Run", false);
	}

	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		Vec3 Pos = m_Script->GetMouseClickPos();
		m_Script->RotatetoClick(Pos);
		GetFSM()->ChangeState(L"Slash_L", 0.1f, L"Slash_L", false);
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
			break;
		case PLAYER_PROJECTILE_TYPE::MAGIC:
			GetFSM()->ChangeState(L"Magic", 0.03f, L"Magic", true);
			break;
		case PLAYER_PROJECTILE_TYPE::BOMB:
			GetFSM()->ChangeState(L"Bomb", 0.03f, L"Bomb", true);
			break;
		case PLAYER_PROJECTILE_TYPE::HOOK:
			GetFSM()->ChangeState(L"Hook", 0.03f, L"Hook", true);
			break;
		default:
			break;
		}
	}


	

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
