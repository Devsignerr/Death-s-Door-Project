#include "pch.h"
#include "TPlayerHook.h"
#include "CPlayerScript.h"
#include "CPlayerHook.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CFSM.h>

void TPlayerHook::Fly()
{
	if (m_BulletLimit == false)
	{
		((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->SetHookDir(((CPlayerScript*)GetScript())->GetPlayerFront());

		((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->SetActive(true);
		Vec3 PlayerPos = GetObj()->Transform()->GetLocalPos();
		PlayerPos.y += 130.f;

		CPlayerScript::m_pHook->Transform()->SetLocalPos(PlayerPos);

		Vec3 Rot = CPlayerScript::GetPlayerRot();
		CPlayerScript::m_pHook->Transform()->SetLocalRot(Rot);

		CPlayerScript::m_pHook->DisconnectWithParent();
		CPlayerScript::m_pHook->RegisterAsParentObj();


		m_BulletLimit = true;
	}
}

void TPlayerHook::update()
{
	CAnimator3D* CurAni = GetObj()->Animator3D();
	UINT iCurClipIdx = CurAni->GetClipIdx();

	if (KEY_NONE(KEY_TYPE::RBTN))
	{
		Fly();
	}


	if(((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->IsHooked())
	{
		GetFSM()->ChangeState(L"HookFly", 0.03f, L"HookFly", false);
	}

	else
	{
		if (KEY_HOLD(KEY_TYPE::RBTN))
		{
			Vec3 Pos = m_Script->GetMouseClickPos();
			m_Script->RotatetoClick(Pos);
		}
	}

	if (((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->IsFailed())
	{
		GetFSM()->ChangeState(L"Idle", 0.03f, L"Idle", false);
		((CPlayerHook*)CPlayerScript::m_pHook->GetScript())->Destroy();
		CPlayerScript::m_pHook = nullptr;
	}

}

void TPlayerHook::Enter()
{
	m_Script = (CPlayerScript*)GetScript();
	CPlayerScript::m_Weapon->MeshRender()->Activate(false);
}

void TPlayerHook::Exit()
{
	m_BulletLimit = false;
}

TPlayerHook::TPlayerHook()
	: m_Script(nullptr)
	, m_BulletLimit(false)
{
}

TPlayerHook::~TPlayerHook()
{
}
