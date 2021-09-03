#include "pch.h"
#include "TPlayerHookFly.h"
#include "CPlayerScript.h"
#include "CPlayerHook.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerHookFly::update()
{
	Vec3 HookPos = ((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->GetHookedPos();
	Vec3 PlayerPos = CPlayerScript::GetPlayerPos();

	Vec3 Dir = HookPos - PlayerPos;
	Dir.Normalize();

	PlayerPos += Dir * m_fFlySpeed*fDT;

	CPlayerScript* pPlayer = CPlayerScript::GetPlayer();
	GetObj()->Transform()->SetLocalPos(PlayerPos);

	if (((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->IsArrived())
	{
		((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->Destroy();
		CPlayerScript::m_pHook = nullptr;

		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
	}
}

void TPlayerHookFly::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
}

void TPlayerHookFly::Exit()
{
	CPlayerScript::m_Weapon->MeshRender()->Activate(true);
}

TPlayerHookFly::TPlayerHookFly():
	m_fFlySpeed(4000.f)
{
}

TPlayerHookFly::~TPlayerHookFly()
{
}
