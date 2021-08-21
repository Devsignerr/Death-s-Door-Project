#include "pch.h"
#include "TPlayerCharge_L.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerCharge_L::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();


	if (1310 >= CurAni->GetFrameIdx() && true == m_IsChargeStay)
	{
		if (KEY_NONE(KEY_TYPE::MBTN))
		{
			GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
		}
	}

	if (1310 < CurAni->GetFrameIdx())
	{
		if (KEY_NONE(KEY_TYPE::MBTN))
		{
			m_IsChargeStay = true;
		}

		if (m_IsChargeStay)
		{
			GetFSM()->ChangeState(L"Charge_Attack_R", 0.03f, L"Charge_Attack_R", false);
		}
	}


	if (CurAni->GetMTAnimClip()->at(iCurClipIdx).bFinish == true)
	{
		GetFSM()->ChangeState(L"Charge_Max_L", 0.03f, L"Charge_Max_L", false);
	}
}

void TPlayerCharge_L::Enter()
{
}

void TPlayerCharge_L::Exit()
{
	m_IsChargeStay = false;
}

TPlayerCharge_L::TPlayerCharge_L()
	: m_IsChargeStay(false)
{
}

TPlayerCharge_L::~TPlayerCharge_L()
{
}
