#include "pch.h"
#include "TPlayerCharge_R.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerCharge_R::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (1430 >= CurAni->GetFrameIdx() && true == m_IsChargeStay)
	{
		if (KEY_AWAY(KEY_TYPE::MBTN))
		{
			GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
		}
	}

	if (1430 < CurAni->GetFrameIdx())
	{
		if (KEY_NONE(KEY_TYPE::MBTN))
		{
			m_IsChargeStay = true;
		}

		if (m_IsChargeStay)
		{
			GetFSM()->ChangeState(L"Charge_Attack_L", 0.03f, L"Charge_Attack_L", false);
		}
	}

	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Charge_Max_R", 0.03f, L"Charge_Max_R", false);
	}
}

void TPlayerCharge_R::Enter()
{
}

void TPlayerCharge_R::Exit()
{
	m_IsChargeStay = false;
}

TPlayerCharge_R::TPlayerCharge_R()
	: m_IsChargeStay(false)
{
}

TPlayerCharge_R::~TPlayerCharge_R()
{
}
