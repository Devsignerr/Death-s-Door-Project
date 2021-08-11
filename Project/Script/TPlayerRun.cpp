#include "pch.h"
#include "TPlayerRun.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CFSM.h>

void TPlayerRun::update()
{
	if (KEY_TAP(KEY_TYPE::LBTN))
	{
		GetFSM()->ChangeState(L"Idle", 0.2f, L"Idle", false);
	}


}

TPlayerRun::TPlayerRun()
{
}

TPlayerRun::~TPlayerRun()
{
}
