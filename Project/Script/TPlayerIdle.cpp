#include "pch.h"
#include "TPlayerIdle.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CFSM.h>

void TPlayerIdle::update()
{
	if (KEY_TAP(KEY_TYPE::RBTN))
	{
		GetFSM()->ChangeState(L"Run", 0.2f, L"Run", false);
	}
}

void TPlayerIdle::lateupdate()
{
}

TPlayerIdle::TPlayerIdle()
{
}

TPlayerIdle::~TPlayerIdle()
{
}
