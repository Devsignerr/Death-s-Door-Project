#include "pch.h"
#include "TPlayerChargeMax_L.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerChargeMax_L::update()
{
	if (KEY_NONE(KEY_TYPE::MBTN))
	{
		GetFSM()->ChangeState(L"Charge_Attack_R", 0.03f, L"Charge_Attack_R", false);
	}

}

void TPlayerChargeMax_L::Enter()
{
}

void TPlayerChargeMax_L::Exit()
{
}

TPlayerChargeMax_L::TPlayerChargeMax_L()
{
}

TPlayerChargeMax_L::~TPlayerChargeMax_L()
{
}
