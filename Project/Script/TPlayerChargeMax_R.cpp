#include "pch.h"
#include "TPlayerChargeMax_R.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerChargeMax_R::update()
{
	if (KEY_NONE(KEY_TYPE::MBTN))
	{
		GetFSM()->ChangeState(L"Charge_Attack_L", 0.03f, L"Charge_Attack_L", false);
	}
}

void TPlayerChargeMax_R::Enter()
{
}

void TPlayerChargeMax_R::Exit()
{
}

TPlayerChargeMax_R::TPlayerChargeMax_R()
{
}

TPlayerChargeMax_R::~TPlayerChargeMax_R()
{
}
